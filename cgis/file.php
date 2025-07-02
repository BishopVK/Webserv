<?php
echo "Content-type: text/html\n\n";

if (isset($_FILES['file'])) {
    $tmp = $_FILES['file']['tmp_name'];
    $name = basename($_FILES['file']['name']);
    $destination = "/home/isainz-r/Webserv/cgis/" . $name;

    if (move_uploaded_file($tmp, $destination)) {
        echo "✅ File uploaded successfully to $destination";
    } else {
        echo "❌ Failed to move uploaded file.";
    }
} else {
    echo "⚠️ No file received.";
}
?>