# Maquina-virtual-de-Self
Maquina virtual de Self- Ejercicio Final

Instalacion:
  Librerias necesarias gtkmm-3.0:
    Instalacion:
      Debian GNU/Linux:
      sudo apt-get install libgtkmm-3.0-dev
      Fuente: http://www.gtkmm.org/es/download.html
  Instalacion:
    git clone https://github.com/Desuque/Maquina-virtual-de-Self.git
    Para instalar el Server:
      cd vm/
      make
      *esto genera el ejecutable llamado server.
      
    Para instalar el Cliente(interfaz grafica):
      *Se necesita instalar gtkmm-3.0*
      estando en el root de nuestro repo
      cd interfaz/
      make
      *esto genera un ejecutable llamado client_self
      
Ejecucion:
  Para ejecutar (con conexion) necesitamos dos consolas(o dos computadoras), una en el directorio root/vm/
  y otra en el directorio root/interfaz.
  Desde la primer consola lanzamos el server ./server
  Desde la segunda consola lanzamos el client ./client_self

Manual: https://docs.google.com/document/d/14r_t-iPO6fAU6HOkm93Y6b_OxCNUwZtQVQs2WfcBTVU/edit
  
