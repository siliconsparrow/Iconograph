package com.example.iconograph

import android.bluetooth.*
import android.bluetooth.le.ScanCallback
import android.bluetooth.le.ScanFilter
import android.bluetooth.le.ScanResult
import android.bluetooth.le.ScanSettings
import android.content.Context
import android.content.Intent
import android.graphics.Color
import android.util.Log
import android.widget.Button
import java.util.*
import kotlin.collections.ArrayList

class BleComm(m: MainActivity) {

    enum class BleState(val str: String) {
        IDLE("IDLE"),
        SCANNING("SCANNING"),
        CONNECTING("CONNECTING"),
        CONNECTED("CONNECTED")
    }

    // Values
    private val UART_SERVICE_UUID = UUID.fromString("6e400001-b5a3-f393-e0a9-e50e24dcca9e")
    private val mainAct = m
    private val logTag = "BLE"
    private val bleDeviceName = "Nordic_UART"

    // Create filter so we will only detect our device and nothing else.
    private val filters = listOf(ScanFilter.Builder().setDeviceName(bleDeviceName).build())
    private val bluetoothAdapter: BluetoothAdapter by lazy {
        val bluetoothManager = mainAct.getSystemService(Context.BLUETOOTH_SERVICE) as BluetoothManager
        bluetoothManager.adapter
    }
    // BLE scanner
    private val bleScanner by lazy {
        bluetoothAdapter.bluetoothLeScanner
    }
    private val scanSettings = ScanSettings.Builder()
        .setScanMode(ScanSettings.SCAN_MODE_LOW_LATENCY)
        .build()
    private val scanCallback = object : ScanCallback() {
        override fun onScanResult(callbackType: Int, result: ScanResult) {
            with(result.device) {
                scanStop()
                log("Found BLE device. Name: ${name ?: "Unnamed"}, address: $address")
                bleConnect(result.device)
            }
        }
    }

    // Variables
    private var state = BleState.IDLE
    //var isScanning: Boolean = false

    init {
        Log.w(logTag, "Init")
    }

    private fun log(msg:String) {
        Log.w(logTag, msg)
    }

    private fun setState(s: BleState) {
        state = s
        log("State: ${s.str}")
        mainAct.setStatusText(s.str)
    }

    fun isBluetoothEnabled(): Boolean {
        return bluetoothAdapter.isEnabled
    }

    fun pause() {
        // TODO: Maybe disconnect BLE connection here?
    }

    fun resume() {
        if(state == BleState.IDLE) {
            scanStart()
        }
    }

    fun destroy() {

    }

    // Start scanning for the bluetooth device.
    private fun scanStart() {
        bleScanner.startScan(filters, scanSettings, scanCallback)
        setState(BleState.SCANNING)
    }

    // Stop scanning for the bluetooth device.
    private fun scanStop() {
        bleScanner.stopScan(scanCallback)
        setState(BleState.IDLE)
    }

    private fun connectionFail() {
        setState(BleState.IDLE)

        // Wait a second or so then restart scanning
        Thread.sleep(1000)
        scanStart()
    }

    private fun printGattTable(services: List<BluetoothGattService>) {
        if(services.isEmpty()) {
            Log.w(logTag, "No services available")
            return
        }
        services.forEach { service->
            val characteristicsTable = service.characteristics.joinToString(
                separator = "\n|--",
                prefix = "|--"
            ) { it.uuid.toString() }
            Log.w(logTag,"\nService ${service.uuid}\nCharacteristics:\n$characteristicsTable")
        }
    }

    private val gattCallback = object : BluetoothGattCallback() {
        override fun onConnectionStateChange(gatt: BluetoothGatt, status: Int, newState: Int) {
            val deviceAddress = gatt.device.address
            if(status == BluetoothGatt.GATT_SUCCESS) {
                if(newState == BluetoothProfile.STATE_CONNECTED) {
                    Log.w(logTag, "Connected to $deviceAddress")
                    Log.w(logTag, "Discovering services...")
                    gatt.discoverServices()
                } else {
                    Log.w(logTag, "Disconnected from $deviceAddress")
                    gatt.close()
                    connectionFail()
                }
            } else {
                Log.w(logTag,"Error $status with connection to $deviceAddress")
                Log.w(logTag, "Disconnected from $deviceAddress")
                gatt.close()
                connectionFail()
            }
        }

        override fun onServicesDiscovered(gatt: BluetoothGatt, status: Int) {
            Log.w(logTag, "Discovered ${gatt.services.size} for ${gatt.device.address}")
            printGattTable(gatt.services)
            setState(BleState.CONNECTED)
        }
    }

    private fun bleConnect(device: BluetoothDevice)  {
        Log.w(logTag,"Connecting to ${device.address}")
        setState(BleState.CONNECTING)
        device.connectGatt(mainAct, false, gattCallback)
    }


}