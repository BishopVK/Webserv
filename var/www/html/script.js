function toggleMenu() {
  document.getElementById("navLinks").classList.toggle("show");
}

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
		fileNameDisplay.innerHTML = `Selected file: <strong>${fileInput.files[0].name}</strong>`;
	} else {
		fileNameDisplay.textContent = '';
	}
});