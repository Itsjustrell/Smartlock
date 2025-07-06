// SPDX-License-Identifier: MIT
pragma solidity ^0.8.19;

import "forge-std/Script.sol";
import "../src/SmartLock.sol";

contract DeployScript is Script {
    function run() external {
        vm.startBroadcast();
        SmartLock smartLock = new SmartLock();
        vm.stopBroadcast();

        console.log("SmartLock deployed at:", address(smartLock));
    }
}