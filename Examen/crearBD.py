import sqlite3

def setup_full_db():
    conn = sqlite3.connect('universidad.db')
    cursor = conn.cursor()

    # 1. Definición de Tablas (Estructura 4NF)
    cursor.executescript('''
        DROP TABLE IF EXISTS Alumnos_Minors;
        DROP TABLE IF EXISTS Inscripciones;
        DROP TABLE IF EXISTS Secciones;
        DROP TABLE IF EXISTS Cursos;
        DROP TABLE IF EXISTS Departamentos;
        DROP TABLE IF EXISTS Alumnos;

        CREATE TABLE Alumnos (
            SNum TEXT PRIMARY KEY,
            DNI TEXT,
            Nombre TEXT,
            Ap_Materno TEXT,
            Ap_Paterno TEXT,
            Fecha TEXT,
            Sexo TEXT,
            Clase TEXT,
            Domicilio TEXT,
            Ciudad TEXT,
            Estado TEXT,
            CP TEXT,
            Major TEXT
        );

        CREATE TABLE Departamentos (
            Depto_ID TEXT PRIMARY KEY,
            Nombre_Depto TEXT,
            Oficina TEXT,
            Telefono TEXT,
            Facultad TEXT
        );

        CREATE TABLE Cursos (
            Curso_Num TEXT PRIMARY KEY,
            Nombre_Curso TEXT,
            Descripcion TEXT,
            Horas INTEGER,
            Nivel TEXT,
            Depto_ID TEXT,
            FOREIGN KEY (Depto_ID) REFERENCES Departamentos(Depto_ID)
        );

        CREATE TABLE Secciones (
            ID_Seccion TEXT PRIMARY KEY,
            Curso_Num TEXT,
            Sec_Num INTEGER,
            Semestre TEXT,
            Anio INTEGER,
            Profesor TEXT,
            FOREIGN KEY (Curso_Num) REFERENCES Cursos(Curso_Num)
        );

        -- 4NF: Tabla independiente para Inscripciones y Grados
        CREATE TABLE Inscripciones (
            ID_Grado TEXT PRIMARY KEY,
            SNum TEXT,
            ID_Seccion TEXT,
            Cal_Num INTEGER,
            Letra TEXT,
            FOREIGN KEY (SNum) REFERENCES Alumnos(SNum),
            FOREIGN KEY (ID_Seccion) REFERENCES Secciones(ID_Seccion)
        );

        -- 4NF: Tabla independiente para Minors (Multivaluados)
        CREATE TABLE Alumnos_Minors (
            SNum TEXT,
            Depto_ID TEXT,
            PRIMARY KEY (SNum, Depto_ID),
            FOREIGN KEY (SNum) REFERENCES Alumnos(SNum)
        );
    ''')

    # 2. Datos de Alumnos (Los 20 registros)
    alumnos_data = [
        ('S1001','521861420','María Luisa','Araceli','Vega','2005-10-06','M','5to semestre','Boulevard Sur Niño 851','San Marco Antonio','Durango','63265','ART'),
        ('S1002','912416442','Carlota','Ariadna','Nájera','2003-10-07','F','3er semestre','Privada Sur Olivo 683','San Cristal','Querétaro','22487','CSH'),
        ('S1003','777290831','Nicolás','Mitzy','Toledo','2005-05-20','F','1er semestre','Diagonal Sur García 485','San Anel','Sonora','07110','COM'),
        ('S1004','179253543','Eric','Armando','Vargas','2003-12-17','F','7mo semestre','Pasaje Ochoa 286','Nueva Países Bajos','Guerrero','41139','CSH'),
        ('S1005','82002031','María','Fidel','Pantoja','1996-11-07','F','5to semestre','Andador Puebla 544','Vieja Samoa','Baja California','83058','SAL'),
        ('S1006','137616787','Carla','Octavio','Borrego','1997-01-21','M','3er semestre','Periférico Valenzuela 027','Vieja Uganda','Guerrero','04922','SAL'),
        ('S1007','296816353','Ángela','Espartaco','Aguilera','1998-09-05','F','5to semestre','Diagonal Segovia 385','San Rubén','Puebla','54226','ING'),
        ('S1008','555203433','Esteban','Gabriela','Salazar','1995-01-12','F','5to semestre','Pasaje Norte Báez 158','San José Eduardo','Tabasco','33278','ING'),
        ('S1009','133767936','Roberto','Ana','Valencia','1999-03-13','F','1er semestre','Callejón Olmos 941','San Rafaél','Yucatán','82869','SAL'),
        ('S1010','309623581','Óscar','Lourdes','Estévez','2005-12-05','F','5to semestre','Avenida Santacruz 055','San Ana María','Nayarit','97199','ART'),
        ('S1011','131790292','María','Patricio','Olmos','1995-01-18','M','1er semestre','Calzada Lozada 232','Vieja Costa Rica','Colima','04354','INF'),
        ('S1012','60845126','Aurelio','Rodrigo','Arias','2001-06-08','M','5to semestre','Andador Querétaro 489','San Verónica','Guanajuato','86893','CSH'),
        ('S1013','319678542','Ramiro','Alejandro','Alcaraz','2002-10-27','F','1er semestre','Boulevard Vaca 238','Nueva Kazajstán','Baja California Sur','77245','ADM'),
        ('S1014','661794898','Aldonza','José Emilio','Barragán','1999-10-05','F','3er semestre','Ampliación Brito 828','Vieja Iraq','Jalisco','19683','CNY'),
        ('S1015','559149988','Pedro','Beatriz','Longoria','2004-07-05','F','3er semestre','Continuación Guillen 866','San Hermelinda','Tlaxcala','68371','SAL'),
        ('S1016','504169559','Dolores','Abraham','Reyes','2006-12-04','F','7mo semestre','Prolongación Sur Apodaca 426','San Miguel','Distrito Federal','71535','ING'),
        ('S1017','828748077','Jorge','Natividad','Alemán','2001-02-01','M','1er semestre','Circunvalación Distrito Federal 724','Vieja Rep. Dom.','Quintana Roo','96509','ART'),
        ('S1018','617296547','Abraham','Zacarías','Anguiano','2007-05-09','F','3er semestre','Calzada Sur Moya 041','San Irene','Querétaro','62817','ING'),
        ('S1019','942739743','René','Ana María','Medina','2003-07-08','F','5to semestre','Retorno Hidalgo 828','San Virginia','Oaxaca','21284','INF'),
        ('S1020','694465568','Elena','Alfredo','Zedillo','1995-06-03','F','7mo semestre','Andador Tayikistán 789','Vieja India','Chihuahua','87694','CNY')
    ]
    cursor.executemany('INSERT INTO Alumnos VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?)', alumnos_data)

    # 3. Departamentos
    depto_data = [
        ('EDU','Educación','Edificio 5 Of 271','1-798-800-8189','Educación'),
        ('INF','Informática','Edificio 2 Of 170','(454)064-2378','Ingeniería'),
        ('MAT','Matemáticas','Edificio 3 Of 388','1-757-934-6909','Ciencias'),
        ('SAL','Salud','Edificio 1 Of 120','1-964-894-2395','Medicina'),
        ('ADM','Administración','Edificio 1 Of 494','888.204.5266','Contaduría'),
        ('COM','Comunicación','Edificio 3 Of 460','1-448-272-3458','Comunicación'),
        ('ART','Arte y Diseño','Edificio 4 Of 375','1-337-227-9541','Arquitectura'),
        ('ING','Ingeniería Industrial','Edificio 2 Of 100','555-0101','Ingeniería'),
        ('CSH','Ciencias Sociales','Edificio 6 Of 200','555-0102','Sociales')
    ]
    cursor.executemany('INSERT INTO Departamentos VALUES (?,?,?,?,?)', depto_data)

    # 4. Cursos y Secciones (Simplificado para el mapeo)
    cursos = [
        ('EDU-201','Diseño Material','Planeación materiales',4,'Licenciatura','EDU'),
        ('ING-301','Bases de Datos','SQL y Relacional',4,'Posgrado','INF'),
        ('MAT-101','Cálculo','Límites y derivadas',5,'Doctorado','MAT'),
        ('ING-401','Redes','Fundamentos redes',5,'Doctorado','INF'),
        ('EDU-101','Teorías Aprendizaje','Modelos',4,'Licenciatura','EDU'),
        ('SAL-101','Anatomía','Estructura cuerpo',4,'Posgrado','SAL'),
        ('ADM-101','Admin','Principios básicos',4,'Posgrado','ADM'),
        ('COM-201','Periodismo Digital','Contenidos',5,'Licenciatura','COM'),
        ('ADM-301','Finanzas','Análisis financiero',4,'Doctorado','ADM'),
        ('ART-201','Teoría Arquitectura','Historia',4,'Licenciatura','ART')
    ]
    cursor.executemany('INSERT INTO Cursos VALUES (?,?,?,?,?,?)', cursos)

    secciones = [
        ('SEC21','EDU-201',1,'Otoño',2023,'Diana Romero'),
        ('SEC4','ING-301',2,'Primavera',2024,'Rufino Aguirre'),
        ('SEC23','MAT-101',2,'Primavera',2023,'Tania Franco'),
        ('SEC5','ING-401',1,'Primavera',2024,'Asunción Alemán'),
        ('SEC20','EDU-101',1,'Verano',2024,'Sofía Quiñónez'),
        ('SEC14','SAL-101',1,'Primavera',2023,'Horacio Ponce'),
        ('SEC6','ADM-101',3,'Otoño',2023,'Alvaro Ortiz'),
        ('SEC26','COM-201',3,'Otoño',2025,'Margarita Aparicio'),
        ('SEC10','ADM-301',3,'Otoño',2024,'Luis Barbara'),
        ('SEC18','ART-201',1,'Otoño',2023,'Estela Calvillo'),
        ('SEC7','ADM-101',2,'Otoño',2024,'Claudio Puente')
    ]
    cursor.executemany('INSERT INTO Secciones VALUES (?,?,?,?,?,?)', secciones)

    # 5. Inscripciones (Mapeo de calificaciones del archivo)
    inscripciones = [
        ('G120','S1001','SEC21',61,'D'), ('G10','S1002','SEC4',98,'A'),
        ('G17','S1003','SEC23',60,'D'), ('G68','S1004','SEC5',95,'A'),
        ('G121','S1005','SEC20',69,'D'), ('G14','S1006','SEC14',60,'D'),
        ('G110','S1007','SEC21',96,'A'), ('G179','S1008','SEC6',78,'C'),
        ('G192','S1009','SEC14',98,'A'), ('G69','S1010','SEC26',67,'D'),
        ('G101','S1011','SEC21',70,'C'), ('G132','S1012','SEC14',70,'C'),
        ('G130','S1013','SEC10',64,'D'), ('G40','S1014','SEC14',70,'C'),
        ('G33', 'S1016','SEC18',73,'C'), ('G115','S1017','SEC21',79,'C'),
        ('G89', 'S1018','SEC6',60,'D'), ('G59', 'S1019','SEC7',76,'C'),
        ('G85', 'S1020','SEC7',82, 'B')
    ]
    cursor.executemany('INSERT INTO Inscripciones VALUES (?,?,?,?,?)', inscripciones)

    # 6. Minors (Extracción de columnas MinorDept_1, 2, 3)
    minors = [
        ('S1006','EDU'), ('S1007','COM'), ('S1010','SAL'),
        ('S1013','INF'), ('S1014','COM'), ('S1014','ING'),
        ('S1018','MAT'), ('S1018','CSH')
    ]
    cursor.executemany('INSERT INTO Alumnos_Minors VALUES (?,?)', minors)

    conn.commit()
    conn.close()
    print("Base de datos 'universidad.db' creada con éxito en 4NF.")

if __name__ == "__main__":
    setup_full_db()