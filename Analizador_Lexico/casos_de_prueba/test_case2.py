class Persona:
    def __init__(self, nombre, edad):
        self.nombre = nombre
        self.edad = edad

    def saludar(self):
        # Esto es un saludo
        print("Hola")

def funcion_principal():
    persona = Persona("Ana", 30)
