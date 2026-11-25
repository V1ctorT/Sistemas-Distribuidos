import socket
import json
import struct

HOST_SERVIDOR = "127.0.0.1"   # Cambia si tu servidor usa otra IP
PUERTO = 3000

def send_json(sock, data):
    payload = json.dumps(data, ensure_ascii=False).encode("utf-8")
    sock.sendall(struct.pack("!I", len(payload)))
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
    header = recv_exact(sock, 4)
    if not header:
        return None
    length = struct.unpack("!I", header)[0]
    payload = recv_exact(sock, length)
    if not payload:
        return None
    return json.loads(payload.decode("utf-8"))

def enviar_peticion(peticion):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as cliente:
        cliente.connect((HOST_SERVIDOR, PUERTO))
        send_json(cliente, peticion)
        return recv_json(cliente)

def pedir_pares():
    datos = {}
    while True:
        campo = input("Campo (Enter para terminar): ").strip()
        if not campo:
            break
        valor = input(f"Valor para {campo}: ").strip()
        datos[campo] = valor
    return datos

def menu():
    print("\n===== CLIENTE UNIVERSIDAD =====")
    print("1. Consultar tabla")
    print("2. Insertar registro")
    print("3. Actualizar registro")
    print("4. Eliminar registro")
    print("5. Consultar inscripciones de un alumno")
    print("6. Salir")

def main():
    while True:
        menu()
        opcion = input("Elige una opción: ").strip()

        if opcion == "1":
            tabla = input("Tabla: ").strip()
            print("Filtros de búsqueda:")
            filtros = pedir_pares()
            peticion = {
                "accion": "consultar",
                "tabla": tabla,
                "filtros": filtros
            }
            respuesta = enviar_peticion(peticion)
            print("\nRespuesta:")
            print(json.dumps(respuesta, indent=4, ensure_ascii=False))

        elif opcion == "2":
            tabla = input("Tabla: ").strip()
            print("Datos a insertar:")
            datos = pedir_pares()
            peticion = {
                "accion": "insertar",
                "tabla": tabla,
                "datos": datos
            }
            respuesta = enviar_peticion(peticion)
            print("\nRespuesta:")
            print(json.dumps(respuesta, indent=4, ensure_ascii=False))

        elif opcion == "3":
            tabla = input("Tabla: ").strip()
            print("Datos nuevos:")
            datos = pedir_pares()
            print("Filtros para localizar el registro:")
            filtros = pedir_pares()
            peticion = {
                "accion": "actualizar",
                "tabla": tabla,
                "datos": datos,
                "filtros": filtros
            }
            respuesta = enviar_peticion(peticion)
            print("\nRespuesta:")
            print(json.dumps(respuesta, indent=4, ensure_ascii=False))

        elif opcion == "4":
            tabla = input("Tabla: ").strip()
            print("Filtros del registro a eliminar:")
            filtros = pedir_pares()
            peticion = {
                "accion": "eliminar",
                "tabla": tabla,
                "filtros": filtros
            }
            respuesta = enviar_peticion(peticion)
            print("\nRespuesta:")
            print(json.dumps(respuesta, indent=4, ensure_ascii=False))

        elif opcion == "5":
            snum = input("SNum del alumno: ").strip()
            peticion = {
                "accion": "inscripciones_alumno",
                "SNum": snum
            }
            respuesta = enviar_peticion(peticion)
            print("\nRespuesta:")
            print(json.dumps(respuesta, indent=4, ensure_ascii=False))

        elif opcion == "6":
            print("Cliente finalizado")
            break

        else:
            print("Opción no válida")

if __name__ == "__main__":
    main()