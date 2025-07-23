<?php
header('Content-Type: application/json');

$response = [];

if (isset($_FILES['file'])) {
    $tmp = $_FILES['file']['tmp_name'];
    $name = basename($_FILES['file']['name']);
    $destination = "./uploads/" . $name;
    if (move_uploaded_file($tmp, $destination)) {
        $response['success'] = true;
        $response['message'] = "Archivo $name subido correctamente!";
    } else {
        $response['success'] = false;
        $response['message'] = "Error al mover el archivo.";
    }
} else {
    $response['success'] = false;
    $response['message'] = "No se recibió ningún archivo.";
}

echo json_encode($response);

// TESTING ONLY
/* set_time_limit(0); // Evita que PHP mate el script por timeout (por si acaso)
while (true) {
    // No hace nada, solo se queda en bucle infinito
} */
?>