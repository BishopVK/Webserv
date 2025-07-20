<?php
error_log("=== DELETE.PHP invoked, METHOD=". $_SERVER['REQUEST_METHOD']);

// 1) Sólo método DELETE
if ($_SERVER['REQUEST_METHOD'] !== 'DELETE') {
    http_response_code(405);
    exit("Method Not Allowed");
}

// 2) Ruta absoluta del directorio de uploads
$uploadDir = realpath(__DIR__ . '/../upload/uploads');
if (!$uploadDir || !is_dir($uploadDir)) {
    http_response_code(500);
    exit("Upload directory not found");
}

// 3) Leer y sanear input
parse_str(file_get_contents("php://input"), $data);
$filename = basename(trim($data['file'] ?? ''));

// 4) NO se acepta nombre vacío
if ($filename === '') {
    http_response_code(400);
    exit("No file specified");
}

// 5) Construir ruta completa
$targetPath = $uploadDir . DIRECTORY_SEPARATOR . $filename;

// 6) Resolver path real
$realPath = realpath($targetPath);

// 7) Validaciones:
//    a) Debe existir
if (!$realPath || !file_exists($realPath)) {
    http_response_code(404);
    exit("File not found");
}
//    b) Debe ser un archivo, no un directorio
if (!is_file($realPath)) {
    http_response_code(403);
    exit("Not a file");
}
//    c) Debe estar estrictamente dentro de $uploadDir
if (strpos($realPath, $uploadDir) !== 0) {
    http_response_code(403);
    exit("Access denied");
}

// 8) ¡Eliminar!
if (unlink($realPath)) {
    echo "File successfully deleted";
} else {
    http_response_code(500);
    echo "Error deleting file";
}
