#!/bin/bash

# Verificar si se proporciona un directorio como argumento
if [ $# -ne 1 ]; then
    echo "Uso: $0 directorio_destino"
    exit 1
fi

# Directorio de destino proporcionado como argumento
dest_dir="$1"

# Crear el directorio de destino (si no existe) o borrar su contenido
if [ -d "$dest_dir" ]; then
    rm -r "$dest_dir"/*
else
    mkdir -p "$dest_dir"
fi

# Crear los ficheros y enlaces
touch "$dest_dir/fichero1"
echo "1234567890" > "$dest_dir/fichero2"
ln -s "$dest_dir/fichero2" "$dest_dir/enlaceS"
ln "$dest_dir/fichero2" "$dest_dir/enlaceH"
mkdir "$dest_dir/subdir"

# Mostrar atributos de los ficheros y directorio
for file in "$dest_dir/fichero1" "$dest_dir/fichero2" "$dest_dir/enlaceS" "$dest_dir/enlaceH" "$dest_dir/subdir"; do
    stat "$file"
done

# Responder a las preguntas
echo "Preguntas:"
echo "1. ¿Cuántos bloques de disco ocupa el fichero fichero1? ¿Y el fichero fichero2? ¿Cuál es su tamaño en bytes?"
du -h "$dest_dir/fichero1" "$dest_dir/fichero2"

echo "2. ¿Cuál es el tamaño reportado para subdir? ¿Por qué el campo número de enlaces (Links) en este caso es 2?"
du -h "$dest_dir/subdir"
stat "$dest_dir/subdir"
# El campo número de enlaces es 2 porque "." y ".." cuentan como enlaces en un directorio.

echo "3. ¿Comparten número de i-nodo alguno de los ficheros o directorios creados?"
# Para verificar los números de i-nodo, puedes usar el comando "ls" con las opciones "-ia".
ls -ia "$dest_dir"

echo "4. Muestra el contenido de enlaceH y de enlaceS utilizando el comando cat."
cat "$dest_dir/enlaceH"
cat "$dest_dir/enlaceS"

echo "5. A continuación, borra el fichero fichero2 y repite el procedimiento. ¿Puedes acceder en ambos casos al contenido del fichero?"
rm "$dest_dir/fichero2"
cat "$dest_dir/enlaceH"
cat "$dest_dir/enlaceS"

echo "6. Utiliza la orden touch para modificar las fechas de acceso y modificación del fichero enlaceH."
touch "$dest_dir/enlaceH"

echo "7. ¿Qué cambios se observan en la salida de stat tras su ejecución?"
stat "$dest_dir/enlaceH"

echo "8. Investiga a través de la página de manual para modificar únicamente una de dichas fechas (modificación o acceso)."
# Para modificar solo una fecha, puedes usar la opción "--time" de "touch".
# Por ejemplo, para modificar la fecha de acceso:
touch --time=atime "$dest_dir/enlaceH"
# Para modificar la fecha de modificación:
touch --time=mtime "$dest_dir/enlaceH"


Preguntas RESPUESTAS
1. ¿Cuántos bloques de disco ocupa el fichero fichero1? ¿Y el fichero fichero2? ¿Cuál es su tamaño en bytes?
0    ./fichero1
4,0K    ./fichero2
2. ¿Cuál es el tamaño reportado para subdir? ¿Por qué el campo número de enlaces (Links) en este caso es 2?
4,0K    ./subdir
  Fichero: ./subdir
  Tamaño: 4096 		 Bloques: 8      	Bloque E/S: 4096   directorio
Dispositivo: 801h/2049d    Nodo-i: 1329538 	Enlaces: 2
Acceso: (0755/drwxr-xr-x)  Uid: ( 1001/usuarioso)   Gid: ( 1001/usuarioso)
  	Acceso: 2023-10-28 11:40:23.038063053 +0200
Modificación: 2023-10-28 11:40:23.002063052 +0200
  	Cambio: 2023-10-28 11:40:23.002063052 +0200
	Creación: -
3. ¿Comparten número de i-nodo alguno de los ficheros o directorios creados?
1200075 .   1207356 enlaceH  1207355 fichero1  1329538 subdir
1210385 ..  1207357 enlaceS  1207356 fichero2
4. Muestra el contenido de enlaceH y de enlaceS utilizando el comando cat.
1234567890
1234567890
5. A continuación, borra el fichero fichero2 y repite el procedimiento. ¿Puedes acceder en ambos casos al contenido del fichero?
1234567890
cat: ./enlaceS: No existe el fichero o el directorio
6. Utiliza la orden touch para modificar las fechas de acceso y modificación del fichero enlaceH.
7. ¿Qué cambios se observan en la salida de stat tras su ejecución?
  Fichero: ./enlaceH
  Tamaño: 11   		 Bloques: 8      	Bloque E/S: 4096   fichero regular
Dispositivo: 801h/2049d    Nodo-i: 1207356 	Enlaces: 1
Acceso: (0644/-rw-r--r--)  Uid: ( 1001/usuarioso)   Gid: ( 1001/usuarioso)
  	Acceso: 2023-10-28 11:40:23.042063053 +0200
Modificación: 2023-10-28 11:40:23.042063053 +0200
  	Cambio: 2023-10-28 11:40:23.042063053 +0200
	Creación: -
8. Investiga a través de la página de manual para modificar únicamente una de dichas fechas (modificación o acceso).




