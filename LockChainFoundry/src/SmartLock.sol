// SPDX-License-Identifier: MIT
pragma solidity ^0.8.19;

contract SmartLock {
    address public owner;
    mapping(bytes32 => bool) public authorizedUIDs;

    // ===== Events =====
    event UIDRegistered(address indexed by, bytes32 indexed uid);
    event UIDRevoked(address indexed by, bytes32 indexed uid);
    event OwnershipTransferred(address indexed previousOwner, address indexed newOwner); // opsional

    modifier onlyOwner() {
        require(msg.sender == owner, "Not owner");
        _;
    }

    constructor() {
        owner = msg.sender;
        emit OwnershipTransferred(address(0), msg.sender); // opsional untuk tracking
    }

    function registerUID(bytes32 uid) external onlyOwner {
        authorizedUIDs[uid] = true;
        emit UIDRegistered(msg.sender, uid);
    }

    function revokeUID(bytes32 uid) external onlyOwner {
        authorizedUIDs[uid] = false;
        emit UIDRevoked(msg.sender, uid);
    }

    function isAuthorized(bytes32 uid) external view returns (bool) {
        return authorizedUIDs[uid];
    }
}