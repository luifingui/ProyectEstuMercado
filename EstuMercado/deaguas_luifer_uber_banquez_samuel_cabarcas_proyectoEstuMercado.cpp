

#include<iostream>
#include<vector>
#include<fstream>
#include<cstring>
using namespace std;

struct Estudiante {
    char cedula[20];
    char nombre[50];
    int grado;
    int saldo;
};
    
struct Producto {
    char codigo[20];
    char nombre[50];
    int precio;
    int cantidad;
};

struct Compra {
    char cedulaEstudiante[20];
    char fecha[20];
    char codigoProducto[20];
    int valor;
};

vector<Estudiante> estudiantes;
vector<Producto> productos;
vector<Compra> compras;

/*FUNCIONES PARA ESTUDIANTES*/

//esta funcion guarda cada estudiante en el archivo
void GuardarArchivoEstudiante(const Estudiante& e){

    fstream file("estudiantes.dat", ios::out | ios::app | ios::binary);
    
    if (!file.is_open()) {
        cout<<"Error al abrir archivo.\n";
        return;
    }   
    
    file.write((char*) &e, sizeof(Estudiante));
    file.close(); 
};

bool validarCedula(const char cedula[20]){
    for(int i = 0; i < estudiantes.size(); i++ ){
        if(strcmp(estudiantes[i].cedula, cedula) == 0){
              return true;
        }   
    }  
    
    return false; 
}

void RegistrarEstudiantes(){
    
    cout<<"----------------------\n";
    cout<<"REGISTRO DE ESTUDIANTE\n";
    cout<<"----------------------\n";
        
    Estudiante e;
    
    cout<<"Digite cedula de estudiante: ";
    cin>>e.cedula;
    cin.ignore();
    
    // se usa la funcion para validar   
    if (validarCedula(e.cedula)) {
        cout << "Ya existe un estudiante con esa cedula.\n";
        return; 
    }
    
    cout<<"Digite nombre de estudiante: ";
    cin.getline(e.nombre,50);
    
    cout<<"Digite grado de estudiante: ";
    cin>>e.grado;
    
    e.saldo = 5000;
    
    estudiantes.push_back(e);
    
    GuardarArchivoEstudiante(e);
    
    cout << "\nEstudiante registrado.\n";
}


/*FUNCIONES PARA PRODUCTOS*/

//funcion para guardar en archivo binario los productos
void GuardarArchivoPro(const Producto& p){
    fstream file("productos.dat", ios::out | ios::app | ios::binary);
    
    if(!file.is_open()){
        cout<<"Error al abrir archivo";
        return;
    }
    
    file.write((char*) &p, sizeof(Producto));
    file.close();
}

bool validarProducto(const char codigo[20]){
    
    for(int i = 0; i < productos.size(); i++){
        if(strcmp(productos[i].codigo, codigo)==0){
            return true;
        }
    }
    
    return false;
}

void AgregarProducto(){
    cout<<"--------------------\n";
    cout<<"REGISTRO DE PRODUCTO\n";
    cout<<"--------------------\n";
        
    Producto p;
    
    cout<<"Digite codigo del Producto: ";
    cin>>p.codigo;
    cin.ignore();
    if(validarProducto(p.codigo)){
        cout<<"Ya hay un producto con este codigo.\n";
        return;
    }
    
    cout<<"Digite nombre del producto: ";
    cin.getline(p.nombre,50);
    
    cout<<"Digite precio del producto: ";
    cin>>p.precio;
    
    cout<<"Digite cantidad en stock: ";
    cin>>p.cantidad;
    
    
    productos.push_back(p);
    
    GuardarArchivoPro(p);
    
    cout << "\nProducto agregado.\n"; 
}

/*FUNCIONES PARA REALIZAR COMPRAS*/

void guardarCompraArchivo(const Compra& c){
    
    fstream file("compras.dat", ios::out | ios::app | ios::binary);
    
    if(!file.is_open()){
        cout<<"Error al abrir el archivo.\n";
        return;
    }
    
    file.write((char*) &c, sizeof(Compra));
    file.close();
}


void RealizarCompra(){
    Compra c;
    int cantidad = 1;
    int estu = 0;
    int produc = 0;
    
    cout<<"Digite cedula del estudiante: ";
    cin>>c.cedulaEstudiante;
    
    cout<<"Digite codigo de producto a comprar: ";
    cin>>c.codigoProducto;
    
    cout<<"Digite la cantidad que desea comprar: ";
    cin>>cantidad;
    cin.ignore();
    
    cout<<"Digite la fecha de la compra (DD/MM/AA): ";
    cin.getline(c.fecha,20);
    
    //validar estudiante y producto
    if(!validarCedula(c.cedulaEstudiante) || !validarProducto(c.codigoProducto)){
            cout<<"cedula o producto invalidos, intente nuevamente\n";
            return;
    }
    
    //validar stock y sacar cual producto es
    for(int i = 0; i < productos.size();i++){
        
        if(strcmp(productos[i].codigo,c.codigoProducto) == 0){
            
            //sacamos una indentificacion para el producto asi ya sabemos su posicion siempre
            produc = i;
            
            if(productos[i].cantidad < cantidad){
                cout<<"No hay stock suficiente de ese producto.";
                return;
            }else{        
                    c.valor = productos[produc].precio * cantidad;
                    
                    for(int i=0;i<estudiantes.size();i++){
                        if(strcmp(estudiantes[i].cedula, c.cedulaEstudiante) == 0){
                            estu = i;
                            if(estudiantes[i].saldo >= c.valor){
                                cout<<"Saldo suficiente.\n";
                                estudiantes[i].saldo -= c.valor;
                            }else{
                                cout<<"Saldo insuficiente.\n";
                                return;
                            }
                            
                            break;
                        }
                    }
                        
                    //reducir stock
                    productos[produc].cantidad -= cantidad;
                    compras.push_back(c);
                    guardarCompraArchivo(c);
                    cout<<"Compra realizada correctamente\n";                         
            }
            
            break;
            
        }
    }   
}


/*Funcion para recargar saldo*/
void ActualizarASaldoArchivo(){
	fstream file("estudiantes.dat", ios::out|ios::binary);
	
	if(!file.is_open()){
		cout<<"Error al abrir archivo.\n";
		return;
	}
	
	for(int i = 0; i<estudiantes.size(); i++){
		file.write((char*) &estudiantes[i], sizeof(Estudiante));
	}
	file.close();
}

void RecargarSaldo(){
	char cedula[20];
	int recarga = 0;	
	int estudiante=0;
	cout<<"-------------------\n";
	cout<<"  RECARGAR SALDO  ";
	cout<<"-------------------\n";
	
	cout<<"Digite la cedula del estudiante: ";
	cin>>cedula;
	
	if (!validarCedula(cedula)) {
        cout << "Cedula invalida.\n";
        return; 
    }	
    
    cout<<"Digite la cantidad de saldo a recargar (máximo $500.000): ";
    cin>>recarga;
    
    
    if(recarga > 500000 || recarga <= 0){
    	cout<<"ERROR: Valor de recarga invalido. intente nuevamente\n";
    	return;
	}
    
    for(int i=0;i<estudiantes.size();i++){
    	if(strcmp(estudiantes[i].cedula,cedula)==0){
    		estudiante = i;
    		estudiantes[estudiante].saldo += recarga;
    		break;
		}
	}
	
	ActualizarASaldoArchivo();
	cout<<"Recarga exitosa.\n";
	
} 

/*FUNCIONES ELIMINAR ESTUDIANTE*/
//GUARDAR ELIMINADOS EN ARCHIVO TXT
void GuardarTXT(Estudiante e){
	
	fstream file("retiros.txt", ios::out|ios::app );
	
	if(!file.is_open()){
		cout<<"Error al abrir archivo.\n";
		return;
	}
	
	file << e.cedula << ", " << e.nombre << ", " << e.grado << ", " << e.saldo << "\n";
	file.close();
	
	
}

void EliminarEstudiante(){
	char cedula[20];
	Estudiante eliminado;
	bool encontrado = false;
	cout<<"-----------------------------\n";
	cout<<"  	ELIMINAR ESTUDIANTE		";
	cout<<"-----------------------------\n";
	
	cout<<"Digite la cedula del estudiante: ";
	cin>>cedula;
	
	if (!validarCedula(cedula)) {
        cout << "Cedula invalida.\n";
        return; 
    }	
    
    for(int i=0;i<estudiantes.size();i++){
    	if(strcmp(estudiantes[i].cedula,cedula)==0){
    		eliminado = estudiantes[i]; 
			estudiantes.erase(estudiantes.begin() + i);
			encontrado = true;
    		break;
		}
	}
	
	if(encontrado){
		GuardarTXT(eliminado);
		ActualizarASaldoArchivo();
		cout<<"Estudiante eliminado.\n";   
	}else{
		cout<<"Estudiante no encontrado.\n";
	}
 

}

/*FUNCIONES PARA CONSULTAS*/
void ComprasPorEstudiante(char cedula[20]){
	bool encontrado = false;
	
	for(int i = 0; i< compras.size(); i++){
		if(strcmp(compras[i].cedulaEstudiante, cedula) == 0){
            cout << "\n-------------------------------------\n";
            cout << "            DETALLE DE COMPRA         \n";
            cout << "-------------------------------------\n";
            cout << " Fecha   :  " << compras[i].fecha << "\n";
            cout << " Producto:  " << compras[i].codigoProducto << "\n";
            cout << " Valor   : $" << compras[i].valor << "\n";
            cout << "-------------------------------------\n";

            encontrado = true;			
		}
	}
	
	if(!encontrado){ 
		cout << "\nNo se encontraron compras para esta cedula.\n";
	}
	 cout << "=====================================\n\n";
}

void  consultaEstudiante(){
	char cedula[20];

	bool encontrado = false;
	cout<<"-----------------------------\n";
	cout<<"  	CONSULTA DE ESTUDIANTE	  ";
	cout<<"-----------------------------\n";
	
	cout<<"Digite la cedula del estudiante: ";
	cin>>cedula;
	
	if (!validarCedula(cedula)) {
        cout << "Cedula invalida.\n";
        return; 
    }	
    
	for (int i = 0; i < estudiantes.size(); i++) {
	    if (strcmp(estudiantes[i].cedula, cedula) == 0) {
	        cout << "\n=====================================\n";
	        cout << "        INFORMACION DEL ESTUDIANTE     \n";
	        cout << "=====================================\n";
	        cout << " Cedula : " << estudiantes[i].cedula << "\n";
	        cout << " Nombre : " << estudiantes[i].nombre << "\n";
	        cout << " Grado  : " << estudiantes[i].grado << "\n";
	        cout << " Saldo  : $" << estudiantes[i].saldo << "\n";
	        cout << "=====================================\n\n";
	
	        encontrado = true;
	        break;
	    }
	}
	
	if(encontrado){
		 ComprasPorEstudiante(cedula);
		cout<<"Consulta finalizada con exito.\n";   
	}else{
		cout<<"Estudiante no encontrado.\n";
	}
 
	
}


void ProductosInventario(){
	
		if (productos.empty()) {
			
		        cout << "\n-----------------------------------------\n";
		        cout << "            INVENTARIO VACIO\n";
		        cout << "-----------------------------------------\n\n";
		        return;
		}
		
		    cout << "\n=========================================\n";
		    cout << "          INVENTARIO DE PRODUCTOS\n";
		    cout << "=========================================\n\n";
		
		for (int i = 0; i < productos.size(); i++) {
		        cout << "-----------------------------------------\n";
		        cout << "Producto #" << i + 1 << "\n";
		        cout << "-----------------------------------------\n";
		        cout << "Codigo:         " << productos[i].codigo << "\n";
		        cout << "Nombre:         " << productos[i].nombre << "\n";
		        cout << "Precio:         $" << productos[i].precio << "\n";
		        cout << "Cantidad:       " << productos[i].cantidad << "\n";
		        cout << "-----------------------------------------\n\n";
	    }		
	
	
	  cout << "=====================================\n";

}
void EstudiantesSaldo() {

    if (estudiantes.empty()) {
        cout << "\n-----------------------------------------\n";
        cout << "            LISTADO VACIO\n";
        cout << "-----------------------------------------\n\n";
        return;
    }

    cout << "\n=========================================\n";
    cout << "      ESTUDIANTES CON SALDO BAJO\n";
    cout << "=========================================\n\n";

    bool encontrado = false;

    for (int i = 0; i < estudiantes.size(); i++) {

        if (estudiantes[i].saldo < 5000) {

            cout << "-----------------------------------------\n";
            cout << "Estudiante #" << i + 1 << "\n";
            cout << "-----------------------------------------\n";
            cout << "Cedula:   " << estudiantes[i].cedula << "\n";
            cout << "Nombre:   " << estudiantes[i].nombre << "\n";
            cout << "Grado:    " << estudiantes[i].grado << "\n";
            cout << "Saldo:   $" << estudiantes[i].saldo << "\n";
            cout << "-----------------------------------------\n\n";

            encontrado = true;
        }
    }

    if (!encontrado) {
        cout << "No hay estudiantes con saldo menor a 5000.\n\n";
    }

    cout << "=====================================\n";
}


void Consultas(){
	char op;
	 
    while(true){
	
	    cout<<"a. Consultar estudiante\n";
	    cout<<"b. Listar productos del inventario\n";
	    cout<<"c. Estudiantes con saldo menor a $5.000\n";
	    cout<<"d. Generar archivo retiros.txt con estudiantes eliminados\n";
	    cout<<"e. Menu principal\n";
	    cout<<"Opcion: ";
	    
	    cin>>op;
	    cin.ignore();
	    
	    switch(op){
	        case 'a':
	        case 'A':
	            consultaEstudiante();
	            break;

	        case 'b':
	        case 'B':
	            ProductosInventario();
	            break;

	        case 'c':
	        case 'C':
	            EstudiantesSaldo();
	            break;

	        case 'd':
	        case 'D':
				cout<<"El archivo retiros.txt ya se genera automaticamente al eliminar estudiantes.\n"; 
	            break;

	        case 'e':
	        case 'E':
	            cout<<"Volviendo al menu principal.\n";
	            return;   // para salir de Consultas()

	        default:
	            cout<<"Opcion no valida.\n";
	    }
	}
}



/*MENÚ PRINCIPAL*/
int main(){
     
    int op;
     
    do{
        
        cout<<"1. Registrar estudiante\n";
        cout<<"2. Agregar producto al inventario\n";
        cout<<"3. Realizar compra\n";
        cout<<"4. Recargar saldo estudiante\n";
        cout<<"5. Eliminar estudiante\n";
        cout<<"6. Consultas\n";
        cout<<"7. Salir\n";
        cout<<"Opcion: ";
        
        cin>>op;
        cin.ignore();
        
        switch(op){
            case 1:  RegistrarEstudiantes(); break;
            case 2:  AgregarProducto();      break;
            case 3:  RealizarCompra();		 break;
            case 4:  RecargarSaldo();        break;
            case 5:  EliminarEstudiante();   break;
            case 6:  Consultas();            break;
            case 7:  cout<<"saliendo...\n";  break;
                
            default: cout<<"Opcion no valida\n";         
        }             
    }while(op != 7);
}
