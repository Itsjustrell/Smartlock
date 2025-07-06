const express = require("express");
const { ethers } = require("ethers");
require("dotenv").config();
const cors = require("cors");
const abi = require("./abi/SmartLock.json");

const app = express();
app.use(cors());
app.use(express.json());

// Setup Conection to Blockchain
const provider = new ethers.JsonRpcProvider(process.env.RPC_URL);
const wallet = new ethers.Wallet(process.env.PRIVATE_KEY, provider);
const contract = new ethers.Contract(process.env.CONTRACT_ADDRESS, abi, wallet);

app.post("/check-uid", async (req, res) => {
    const { uid } = req.body;
    const uidHash = ethers.keccak256(ethers.toUtf8Bytes(uid));
    const access = await contract.isAuthorized(uidHash);
    res.json({ access });
});

app.post("/register-uid", async (req, res) => {
  try {
    const { uid } = req.body;
    const uidHash = ethers.keccak256(ethers.toUtf8Bytes(uid));
    
    const tx = await contract.registerUID(uidHash);
    await tx.wait();

    res.json({ success: true, txHash: tx.hash });
  } catch (err) {
    console.error("Register error:", err);
    res.status(500).json({ success: false, error: err.message });
  }
});

app.post("/revoke-uid", async (req, res) => {
  try {
    const { uid } = req.body;
    const uidHash = ethers.keccak256(ethers.toUtf8Bytes(uid));

    const tx = await contract.revokeUID(uidHash);
    await tx.wait();

    res.json({ success: true, txHash: tx.hash });
  } catch (err) {
    res.status(500).json({ success: false, error: err.message });
  }
});


app.listen(3000, () => console.log("SmartLock API running on port 3000"));

// If run failde, use "node index.js (NODE_OPTIONS="--dns-result-order=ipv4first" node index.js)"