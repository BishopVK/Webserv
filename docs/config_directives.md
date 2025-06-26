# Webserv – Configuración: Server vs Location

## 📌 ¿Dónde puede definirse cada directiva?

| Directiva                | ¿En `server`? | ¿En `location`? | Comentario |
|--------------------------|:-------------:|:----------------:|------------|
| `listen`                 | ✅            | ❌               | Obligatoria en `server`, define IP:puerto |
| `server_name`            | ✅            | ❌               | Para virtual hosting (`Host:`) |
| `error_page`             | ✅            | ✅               | Puede declararse global y sobreescribirse por ruta |
| `client_max_body_size`   | ✅            | ✅               | Puede sobreescribirse por `location` |
| `root`                   | ✅            | ✅               | `location` tiene prioridad sobre `server` |
| `index`                  | ✅            | ✅               | Se puede definir por defecto global y luego por ruta |
| `autoindex`              | ✅            | ✅               | Igual que `index`, puede definirse globalmente |
| `limit_except`           | ✅            | ✅               | Permite restringir métodos en ambos niveles |
| `methods` (`GET`, etc.)  | ❌            | ✅               | En NGINX se usa `limit_except`, no `methods` |
| `upload_store`           | ❌            | ✅               | Específico de cada ruta |
| `upload_enable`          | ❌            | ✅               | Flag por ruta |
| `return 301 ...`         | ❌            | ✅               | Redirecciones HTTP por ubicación |
| `cgi_pass`               | ❌            | ✅               | Ruta al ejecutable CGI para esa ubicación |
| `cgi_extension`          | ❌            | ✅               | Extensión de archivo que activa el CGI |

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
