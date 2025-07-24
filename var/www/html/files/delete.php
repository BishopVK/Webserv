<?php
error_log("=== DELETE.PHP invoked, METHOD=". $_SERVER['REQUEST_METHOD']);

// 1) Only DELETE method
if ($_SERVER['REQUEST_METHOD'] !== 'DELETE') {
    http_response_code(405);
    exit("Method Not Allowed");
}

// 2) Absolute path to the uploads directory
$uploadDir = realpath(__DIR__ . '/../upload/uploads');
if (!$uploadDir || !is_dir($uploadDir)) {
    http_response_code(500);
    exit("Upload directory not found");
}

// 3) Read and clean input
parse_str(file_get_contents("php://input"), $data);
$filename = basename(trim($data['file'] ?? ''));

// 4) Empty names are NOT accepted
if ($filename === '') {
    http_response_code(400);
    exit("No file specified");
}

// 5) Build complete route
$targetPath = $uploadDir . DIRECTORY_SEPARATOR . $filename;

// 6) Resolve real path
$realPath = realpath($targetPath);

// 7) Validations:
//    a) It must exist
if (!$realPath || !file_exists($realPath)) {
    http_response_code(404);
    exit("File not found");
}
//    b) It must be a file, not a directory.
if (!is_file($realPath)) {
    http_response_code(403);
    exit("Not a file");
}
//    c) Must be strictly within $uploadDir
if (strpos($realPath, $uploadDir) !== 0) {
    http_response_code(403);
    exit("Access denied");
}

// 8) Eliminate!
if (unlink($realPath)) {
    echo "File successfully deleted";
} else {
    http_response_code(500);
    echo "Error deleting file";
}
