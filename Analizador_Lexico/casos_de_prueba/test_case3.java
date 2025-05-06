class Empleado {
    int id;
    String nombre;

    public Empleado(int id, String nombre) {
        this.id = id;
        this.nombre = nombre;
    }

    void mostrarDatos() {
        #print;
    }
}

class Main {
    public static void main(String[] args) {
        Empleado e = new Empleado(1, "Luis");
    }
}
