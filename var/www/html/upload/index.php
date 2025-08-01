<!DOCTYPE html>
<html lang="es">
<head>
	<meta charset="UTF-8">
	<meta name="viewport" content="width=device-width, initial-scale=1.0">
	<link rel="stylesheet" href="/welcome.css">
	<link rel="stylesheet" href="/upload.css">
	<link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.5.0/css/all.min.css">
	<script defer src="/script.js"></script>
	<title>File Upload</title>
</head>
<body>
	<div id="wrapper">

		<!-- Header -->
		<div id="header-placeholder"></div>
		<script>
			fetch("/header.html")
				.then(response => response.text())
				.then(data => {
					document.getElementById("header-placeholder").innerHTML = data;
				});
		</script>

		<section class="sections">
			<h2>Upload a file to our server</h2>

			<p>Click on the "Choose File" button to upload a file:</p>

			<form id="uploadForm" action="upload.php" method="POST" enctype="multipart/form-data">
				<label for="myFile" class="custom-file-upload">Select file</label>
				<input type="file" id="myFile" name="file" required>
				<input type="submit" value="Upload">
			</form>

			<span id="file-name" class="file-name-display"></span> <!-- Aquí se muestra el nombre -->

			<!-- Popup div -->
			<div id="popup" class="hidden"></div>
		</section>

		<script>
			document.getElementById('uploadForm').addEventListener('submit', function(e) {
				e.preventDefault(); // Evita que recargue la página

				const form = e.target;
				const formData = new FormData(form);

				fetch('/upload/upload.php', {
					method: 'POST',
					body: formData
				})
				.then(response => response.json())
			.then(data => {
				showPopup(data.success ? '✅ ' + data.message : '❌ ' + data.message);
			})
				.catch(error => {
					console.error('Error:', error);
					showPopup('❌ Unexpected error');
				});
			});
		</script>

		<!-- Footer -->
		<div id="footer-placeholder"></div>
		<script>
			fetch("/footer.html")
				.then(response => response.text())
				.then(data => {
					document.getElementById("footer-placeholder").innerHTML = data;
				});
		</script>
	</div>
</body>
</html>
