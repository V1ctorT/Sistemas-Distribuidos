import socket
import json
import sqlite3
import struct
import re

# ==========================================
# CONFIGURACIÓN DEL SERVIDOR
# ==========================================

HOST = "0.0.0.0"
PUERTO = 3000
BASE_DATOS = "universidad.db"

# Cambia este nombre si tu tabla tiene otro nombre.
TABLA_INSCRIPCIONES = "Inscripciones"

# ==========================================
# COMUNICACIÓN TCP Y JSON
# ==========================================

def send_json(sock, data):
    payload = json.dumps(
        data,
        ensure_ascii=False
    ).encode("utf-8")

    # Enviar primero los 4 bytes con la longitud
    sock.sendall(struct.pack("!I", len(payload)))

    # Enviar el contenido JSON
    sock.sendall(payload)


def recv_exact(sock, n):
    data = b""

    while len(data) < n:
        chunk = sock.recv(n - len(data))

        if not chunk:
            return None

        data += chunk

    return data


def recv_json(sock):
    # Recibir cabecera de 4 bytes
    header = recv_exact(sock, 4)

    if not header:
        return None

    # Recuperar longitud del mensaje
    length = struct.unpack("!I", header)[0]

    # Evitar mensajes excesivamente grandes
    if length > 10 * 1024 * 1024:
        raise ValueError("Mensaje demasiado grande")

    # Recibir contenido JSON
    payload = recv_exact(sock, length)

    if payload is None:
        return None

    return json.loads(payload.decode("utf-8"))


# ==========================================
# FUNCIONES AUXILIARES PARA SQLITE
# ==========================================

def conectar_bd():
    conexion = sqlite3.connect(BASE_DATOS)
    conexion.row_factory = sqlite3.Row
    conexion.execute("PRAGMA foreign_keys = ON")
    return conexion


def validar_identificador(nombre):
    """
    Comprueba que un nombre de tabla o columna
    tenga un formato válido.
    """

    if not isinstance(nombre, str):
        raise ValueError("Nombre de tabla o columna no válido")

    if not re.fullmatch(r"[A-Za-z_][A-Za-z0-9_]*", nombre):
        raise ValueError(f"Identificador no válido: {nombre}")

    return '"' + nombre + '"'


def obtener_tabla(cursor, tabla):
    """
    Comprueba que la tabla exista en la base de datos.
    """

    cursor.execute(
        """
        SELECT name
        FROM sqlite_master
        WHERE type = 'table' AND name = ? COLLATE NOCASE
        """,
        (tabla,)
    )

    resultado = cursor.fetchone()

    if resultado is None:
        raise ValueError(f"La tabla '{tabla}' no existe")

    return resultado["name"]


def obtener_columnas(cursor, tabla):
    """
    Recupera las columnas existentes de una tabla.
    """

    tabla_sql = validar_identificador(tabla)

    cursor.execute(f"PRAGMA table_info({tabla_sql})")

    return [fila["name"] for fila in cursor.fetchall()]


def validar_campos(campos, columnas):
    """
    Comprueba que los campos enviados por el cliente
    existan realmente en la tabla.
    """

    if not isinstance(campos, dict):
        raise ValueError("Los campos deben enviarse como un objeto JSON")

    resultado = {}

    for campo, valor in campos.items():

        coincidencia = next(
            (col for col in columnas if col.lower() == campo.lower()),
            None
        )

        if coincidencia is None:
            raise ValueError(f"La columna '{campo}' no existe")

        resultado[coincidencia] = valor

    return resultado


def construir_filtros(filtros, columnas):
    """
    Construye condiciones WHERE utilizando parámetros SQL.
    """

    filtros = validar_campos(filtros, columnas)

    condiciones = []
    valores = []

    for campo, valor in filtros.items():

        columna_sql = validar_identificador(campo)

        if valor is None:
            condiciones.append(f"{columna_sql} IS NULL")
        else:
            condiciones.append(f"{columna_sql} = ?")
            valores.append(valor)

    if condiciones:
        return " WHERE " + " AND ".join(condiciones), valores

    return "", valores


# ==========================================
# GESTIÓN DE OPERACIONES SQL
# ==========================================

def gestionar_sql(accion, datos):

    conexion = None

    try:

        conexion = conectar_bd()
        cursor = conexion.cursor()

        # ----------------------------------
        # 1. CONSULTAR REGISTROS
        # ----------------------------------

        if accion == "consultar":

            tabla = datos["tabla"]
            filtros = datos.get("filtros", {})

            tabla = obtener_tabla(cursor, tabla)
            columnas = obtener_columnas(cursor, tabla)

            where, valores = construir_filtros(
                filtros, columnas
            )

            tabla_sql = validar_identificador(tabla)

            consulta = f"SELECT * FROM {tabla_sql}{where}"

            cursor.execute(consulta, valores)

            resultado = [
                dict(fila) for fila in cursor.fetchall()
            ]

            return {
                "status": "success",
                "data": resultado,
                "total": len(resultado)
            }

        # ----------------------------------
        # 2. INSERTAR REGISTROS
        # ----------------------------------

        elif accion == "insertar":

            tabla = obtener_tabla(cursor, datos["tabla"])
            campos = datos["datos"]

            columnas = obtener_columnas(cursor, tabla)

            campos = validar_campos(campos, columnas)

            if not campos:
                raise ValueError("No hay datos para insertar")

            nombres = list(campos.keys())
            valores = list(campos.values())

            columnas_sql = ", ".join(
                validar_identificador(c) for c in nombres
            )

            marcadores = ", ".join(["?"] * len(valores))

            consulta = (
                f"INSERT INTO {validar_identificador(tabla)} "
                f"({columnas_sql}) VALUES ({marcadores})"
            )

            cursor.execute(consulta, valores)

            conexion.commit()

            return {
                "status": "success",
                "msg": "Registro insertado correctamente",
                "id": cursor.lastrowid
            }

        # ----------------------------------
        # 3. ACTUALIZAR REGISTROS
        # ----------------------------------

        elif accion == "actualizar":

            tabla = obtener_tabla(cursor, datos["tabla"])

            nuevos_datos = datos["datos"]
            filtros = datos["filtros"]

            columnas = obtener_columnas(cursor, tabla)

            nuevos_datos = validar_campos(
                nuevos_datos, columnas
            )

            if not nuevos_datos:
                raise ValueError("No hay datos para actualizar")

            if not filtros:
                raise ValueError(
                    "Debes indicar filtros para evitar actualizar "
                    "todos los registros"
                )

            where, valores_filtro = construir_filtros(
                filtros, columnas
            )

            asignaciones = ", ".join(
                f"{validar_identificador(campo)} = ?"
                for campo in nuevos_datos
            )

            valores = list(nuevos_datos.values()) + valores_filtro

            consulta = (
                f"UPDATE {validar_identificador(tabla)} "
                f"SET {asignaciones}{where}"
            )

            cursor.execute(consulta, valores)

            afectados = cursor.rowcount

            conexion.commit()

            return {
                "status": "success",
                "msg": "Registro actualizado correctamente",
                "registros_afectados": afectados
            }

        # ----------------------------------
        # 4. ELIMINAR REGISTROS
        # ----------------------------------

        elif accion == "eliminar":

            tabla = obtener_tabla(cursor, datos["tabla"])

            filtros = datos["filtros"]

            columnas = obtener_columnas(cursor, tabla)

            if not filtros:
                raise ValueError(
                    "Debes indicar filtros para evitar "
                    "eliminar todos los registros"
                )

            where, valores = construir_filtros(
                filtros, columnas
            )

            consulta = (
                f"DELETE FROM {validar_identificador(tabla)}{where}"
            )

            cursor.execute(consulta, valores)

            afectados = cursor.rowcount

            conexion.commit()

            return {
                "status": "success",
                "msg": "Operación de eliminación realizada",
                "registros_eliminados": afectados
            }

        # ----------------------------------
        # 5. CONSULTAR INSCRIPCIONES
        # ----------------------------------

        elif accion == "inscripciones_alumno":

            snum = datos["SNum"]

            tabla = obtener_tabla(
                cursor, TABLA_INSCRIPCIONES
            )

            columnas = obtener_columnas(cursor, tabla)

            columna_snum = next(
                (c for c in columnas if c.lower() == "snum"),
                None
            )

            if columna_snum is None:
                raise ValueError(
                    f"La tabla '{tabla}' no tiene una columna SNum"
                )

            consulta = (
                f"SELECT * FROM {validar_identificador(tabla)} "
                f"WHERE {validar_identificador(columna_snum)} = ?"
            )

            cursor.execute(consulta, (snum,))

            resultado = [
                dict(fila) for fila in cursor.fetchall()
            ]

            return {
                "status": "success",
                "data": resultado,
                "total": len(resultado)
            }

        # ----------------------------------
        # ACCIÓN NO RECONOCIDA
        # ----------------------------------

        else:

            return {
                "status": "error",
                "msg": f"Acción no reconocida: {accion}"
            }

    except Exception as e:

        if conexion:
            conexion.rollback()

        return {
            "status": "error",
            "msg": str(e)
        }

    finally:

        if conexion:
            conexion.close()


# ==========================================
# INICIAR SERVIDOR TCP
# ==========================================

def iniciar_servidor():

    servidor = socket.socket(
        socket.AF_INET,
        socket.SOCK_STREAM
    )

    servidor.setsockopt(
        socket.SOL_SOCKET,
        socket.SO_REUSEADDR,
        1
    )

    servidor.bind((HOST, PUERTO))

    servidor.listen(5)

    print("======================================")
    print("     SERVIDOR UNIVERSITARIO")
    print("======================================")
    print(f"Servidor iniciado en el puerto {PUERTO}")
    print(f"Base de datos: {BASE_DATOS}")
    print("Esperando conexiones de clientes...")
    print("======================================")

    while True:

        canal, direccion = servidor.accept()

        print(f"\nCliente conectado: {direccion}")

        with canal:

            try:

                peticion = recv_json(canal)

                if peticion is None:
                    continue

                print("\nPetición recibida:")
                print(json.dumps(
                    peticion,
                    indent=4,
                    ensure_ascii=False
                ))

                accion = peticion.get("accion")

                if not accion:
                    respuesta = {
                        "status": "error",
                        "msg": "No se especificó ninguna acción"
                    }
                else:
                    respuesta = gestionar_sql(
                        accion,
                        peticion
                    )

                send_json(canal, respuesta)

                print("\nRespuesta enviada:")
                print(json.dumps(
                    respuesta,
                    indent=4,
                    ensure_ascii=False
                ))

            except Exception as e:

                print(f"Error en comunicación: {e}")

                try:
                    send_json(canal, {
                        "status": "error",
                        "msg": str(e)
                    })
                except OSError:
                    pass

            finally:

                print("Conexión finalizada.")


# ==========================================
# EJECUCIÓN PRINCIPAL
# ==========================================

if __name__ == "__main__":
    iniciar_servidor()