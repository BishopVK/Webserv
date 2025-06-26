# Webserv – Configuración: Server vs Location

## 📌 ¿Dónde puede definirse cada directiva?

| Directiva                | ¿En `server`? | ¿En `location`? | ¿Es obligatoria? | Comentario |
|--------------------------|:-------------:|:----------------:|:----------------:|------------|
| `listen`                 | ✅            | ❌               | ✅               | Obligatoria en `server`, define IP:puerto |
| `server_name`            | ✅            | ❌               | ❌               | Para virtual hosting (`Host:`) |
| `error_page`             | ✅            | ✅               | ❌               | Se puede definir por tipo de error, heredable |
| `client_max_body_size`   | ✅            | ✅               | ❌               | Define el tamaño máximo de la petición, heredable |
| `root`                   | ✅            | ✅               | ✅ (al menos en uno) | Requerida para servir archivos; `location` tiene prioridad |
| `index`                  | ✅            | ✅               | ❌               | Archivos predeterminados a servir |
| `autoindex`              | ✅            | ✅               | ❌               | Si no hay `index`, muestra el listado si está activado |
| `limit_except`           | ✅            | ✅               | ❌               | Restringe métodos HTTP permitidos |
| `methods` (`GET`, etc.)  | ❌            | ✅               | ❌               | Alternativa a `limit_except` en Webserv |
| `upload_store`           | ❌            | ✅               | ✅ (si hay upload) | Ruta donde se guardan los archivos subidos |
| `upload_enable`          | ❌            | ✅               | ✅ (si hay upload) | Flag para habilitar subidas |
| `return 301 ...`         | ❌            | ✅               | ❌               | Redirecciones HTTP por ruta |
| `cgi_pass`               | ❌            | ✅               | ✅ (si se usa CGI) | Ruta al ejecutable CGI |
| `cgi_extension`          | ❌            | ✅               | ✅ (si se usa CGI) | Extensión de archivo que activa el CGI |

---

## ✅ Directivas heredadas por `location` si no están redefinidas:

| Directiva               | Heredable | Comentario |
|-------------------------|:---------:|------------|
| `root`                  | ✅        | Define el path base del filesystem |
| `index`                 | ✅        | Archivos predeterminados a servir |
| `autoindex`             | ✅        | Muestra listado si no hay `index` |
| `error_page`            | ✅        | Páginas de error personalizadas |
| `client_max_body_size`  | ✅        | Tamaño máximo permitido para el cuerpo de la petición |

---

## ❌ Directivas que **no** se heredan y deben definirse en el `location`:

| Directiva            | Heredable | Comentario |
|----------------------|:---------:|------------|
| `limit_except`       | ❌        | Si no está, no se aplican restricciones |
| `cgi_pass`           | ❌        | Debe definirse por ubicación |
| `cgi_extension`      | ❌        | Solo activa CGI si está definida en ese `location` |
| `upload_store`       | ❌        | Obligatoria por `location` si quieres aceptar archivos |
| `upload_enable`      | ❌        | Flag por ruta |
| `return`             | ❌        | Redirecciones solo se aplican donde se definen |
