const API_BASE = "INPUT_YOUR-SERVER_NAME_HERE"; // Example "http://192.168.61.59:3000"

function showLoader(show) {
    document.getElementById("loader").classList.toggle("hidden", !show);
    document.getElementById("result").innerText = "";
}

async function handleAction(actionFunc) {
    showLoader(true);
    await actionFunc();
    showLoader(false);
}

async function registerUID() {
    const uid = document.getElementById("uidInput").value;
    if (!uid) return alert("Masukkan UID");

    const res = await fetch(`${API_BASE}/register-uid`, {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ uid })
    });

    const data = await res.json();
    const resultMessage = data.success 
        ? `✅ UID registered\nTx: ${data.txHash}`
        : `❌ Error: ${data.error}`;
    
    document.getElementById("result").innerText = resultMessage;
    if (data.success) {
        alert("UID successfully registered!");
    }
}

async function revokeUID() {
    const uid = document.getElementById("uidInput").value;
    if (!uid) return alert("Masukkan UID");

    const res = await fetch(`${API_BASE}/revoke-uid`, {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ uid })
    });

    const data = await res.json();
    const resultMessage = data.success 
        ? `🧹 UID revoked\nTx: ${data.txHash}`
        : `❌ Error: ${data.error}`;
    
    document.getElementById("result").innerText = resultMessage;
    if (data.success) {
        alert("UID successfully revoked!");
    }
}

async function checkUID() {
    const uid = document.getElementById("uidInput").value;
    if (!uid) return alert("Masukkan UID");

    const res = await fetch(`${API_BASE}/check-uid`, {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ uid })
    });

    const data = await res.json();
    const resultMessage = data.access 
        ? "✅ UID Authorized"
        : "🚫 UID Not Authorized";
    
    document.getElementById("result").innerText = resultMessage;
    if (data.access) {
        alert("UID is authorized!");
    } else {
        alert("UID is not authorized.");
    }
}
