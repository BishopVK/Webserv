<?php
/* echo "Content-type: text/html\n\n";
echo "Files:\n";
print_r($_FILES); 
echo "END OF Files\n";
if (isset($_FILES['file'])) {
    $tmp = $_FILES['file']['tmp_name'];
    $name = basename($_FILES['file']['name']);
    $destination = "./uploads/" . $name;
    if (move_uploaded_file($tmp, $destination)) {
        echo ":marca_de_verificación_blanca: File uploaded successfully to $destination";
    } else {
        echo ":x: Failed to move uploaded file.";
    }
} else {
    echo ":advertencia: No file received.";
} */
?>


<?php
print_r($_FILES); // Imprime el contenido de $_FILES
?>