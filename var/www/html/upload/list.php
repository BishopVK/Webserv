<?php
header('Content-Type: application/json');

// Ignored files list
$ignoreFiles = ['index.html', 'style.css'];

// Ruta a la carpeta "uploads"
$dir = "uploads";

// Escanea la carpeta "uploads"
$files = array_filter(scandir($dir), function($f) use ($ignoreFiles, $dir) {
    return is_file($dir . DIRECTORY_SEPARATOR . $f) && !in_array($f, $ignoreFiles);
});

// Devuelve los archivos en formato JSON
echo json_encode(array_values($files));
?>