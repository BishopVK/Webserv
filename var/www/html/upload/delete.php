<?php
if ($_SERVER['REQUEST_METHOD'] !== 'DELETE') {
    http_response_code(405); // Method Not Allowed
    exit("Method Not Allowed");
}

// Files protected to prevent deletion
$protected_files = ['index.html', 'style.css', 'list.php', 'delete.php'];

parse_str(file_get_contents("php://input"), $data);
$filename = basename($data['file'] ?? '');

if (!$filename || !file_exists($filename)) {
    http_response_code(400);
    exit("File not found");
}

// Aditional protection
if (in_array($filename, $protected_files)) {
    http_response_code(403); // Forbidden
    exit("You are not allowed to delete this file.");
}

/* if ($_SERVER['HTTP_X_REQUESTED_WITH'] !== 'XMLHttpRequest') { // Cabecera de JS
    http_response_code(403);
    exit("Forbidden");
} */

// Delete file
unlink($filename);
echo "File successfully deleted";
?>