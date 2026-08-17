function uploadPoints() {
    const pointsData = [...Array(2*((Math.random()*20)|0))].map(() => Math.random());
    document.querySelector("#points").innerHTML = JSON.stringify(pointsData);
    const binaryData = new Float32Array(pointsData);
    const body = binaryData.buffer;
    const headers = {'Content-Type': 'application/octet-stream' }
    const method = "POST"
    fetch('/upload', {method, body, headers})
        .then(res => res.text())
        .then(result => {
            console.log("upload", result)
        });
}

