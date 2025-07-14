<?php
header('Content-Type: application/json');

// Ignored files list
$ignoreFiles = ['index.html', 'style.css', 'list.php', 'delete.php'];

// Scan current directory
$files = array_filter(scandir("."), function($f) use ($ignoreFiles) {
    return is_file($f) && !in_array($f, $ignoreFiles);
});

// Returns files like a json file
echo json_encode(array_values($files));
?>