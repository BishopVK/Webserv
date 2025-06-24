| Opción de Configuración | ¿Obligatoria? | ¿Se puede definir en `server`? | ¿Se puede definir en `location`? | ¿Cuál prevalece si se define en ambas? | Valor por Defecto (si no se define) según el enunciado/lógica |
| :---------------------- | :------------ | :----------------------------- | :------------------------------- | :------------------------------------- | :----------------------------------------------------------- |
| **`listen` (host:port)** | Sí | Sí (para cada servidor) | No aplica (define la escucha del servidor, no de la ruta) | N/A | No especificado, el programa debe tomar un archivo de configuración o usar una ruta por defecto. Se asume que no hay puertos de escucha por defecto si no se configuran. |
| **`server_name`** | Sí (se puede no configurar) | Sí | No aplica (es para la identificación del servidor) | N/A | El primer servidor para un host:puerto será el predeterminado. |
| **`root` (directorio raíz)** | Sí (implícito para servir contenido estático) | Sí | Sí | La de `location` | No especificado. Implícitamente, si no se define, el servidor no puede servir archivos estáticos. |
| **`index` (archivo por defecto)** | Sí (implícito para directorios) | Sí | Sí | La de `location` | No especificado. Si no se define, al solicitar un directorio, se buscaría el `autoindex`. |
| **`error_page`** | Sí (debe tener por defecto) | Sí | Sí | La de `location` (no explícito, pero es el comportamiento común) | El servidor debe tener páginas de error predeterminadas si no se proporcionan. |
| **`client_max_body_size`** | Sí | Sí | Sí | La de `location` | No especificado. Implícitamente, no hay un límite si no se configura. |
| **`autoindex`** | Sí | Sí | Sí | La de `location` | No especificado. Implícitamente, deshabilitado si no se configura. |
| **Métodos HTTP (GET, POST, DELETE)** | Sí (GET, POST, DELETE obligatorios) | No (se define a nivel de ruta) | Sí | N/A | No especificado. Por lógica, ningún método estaría permitido si no se configura. |
| **Redirección HTTP** | Sí | No (es una acción de ruta) | Sí | N/A | No aplica. |
| **Ejecución de CGI** | Sí | No (es una acción de ruta) | Sí | N/A | No aplica. |
| **Subida de archivos** | Sí | No (es una acción de ruta) | Sí | N/A | No aplica. |