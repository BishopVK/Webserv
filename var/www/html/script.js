function toggleMenu() {
  document.getElementById("navLinks").classList.toggle("show");
}

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
		showPopup('❌ Error inesperado');
	});
});

function showPopup(message) {
	const popup = document.getElementById('popup');
	popup.textContent = message;
	popup.classList.remove('hidden');
	setTimeout(() => {
		popup.classList.add('hidden');
	}, 3000);
}

document.getElementById('myFile').addEventListener('change', function() {
	const fileInput = this;
	const fileNameDisplay = document.getElementById('file-name');

	if (fileInput.files.length > 0) {
		fileNameDisplay.innerHTML = `Archivo seleccionado: <strong>${fileInput.files[0].name}</strong>`;
	} else {
		fileNameDisplay.textContent = '';
	}
});