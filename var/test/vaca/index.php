#!/usr/bin/php-cgi
<?php
// Simple PHP CGI script
echo "Content-type: text/html\n\n";

$name = isset($_GET["name"]) ? htmlspecialchars($_GET["name"]) : "Guest";
$age = isset($_POST["age"]) ? htmlspecialchars($_POST["age"]) : "0";
//$name = $_GET['name'] ?? $_POST['name'] ?? "Guest";

echo "<html><body>";
echo "<h1>Hello, $name, $age!</h1>";
echo "<h1>Hello from PHP CGI!</h1>";
echo "</body></html>";



?>
