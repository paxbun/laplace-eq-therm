// Copyright (c) 2021 Chanjung Kim. All rights reserved.
// Licensed under the MIT License.

//! Implements functions used to communicate with Arduino.

use serialport::{SerialPort, SerialPortType};

/// Represents an Arduino device.
pub struct Arduino {
    port: Box<dyn SerialPort>,
}

impl Arduino {
    /// Opens the given serial port.
    ///
    /// # Arguments
    ///
    /// * `port_name`: Name of the serial comm device, e.g. `/dev/ttyUSB0`, `COM3`, etc.
    pub fn new(port_name: &str) -> Result<Self, ()> {
        Ok(Arduino {
            port: serialport::new(port_name, 9600).open().map_err(|_| ())?,
        })
    }

    /// Returns all Arduino devices connected to this PC.
    pub fn get_available_ports() -> Vec<String> {
        let ports = serialport::available_ports().unwrap_or(vec![]);
        let ports = ports
            .into_iter()
            .filter(|info| {
                if let SerialPortType::UsbPort(info) = &info.port_type {
                    // 0x2341 is Arduino Vendor ID
                    info.vid == 0x2341
                } else {
                    false
                }
            })
            .map(|info| info.port_name)
            .collect();

        ports
    }

    /// Wait for the device and retrieve the temperature information.
    /// If the device sent a string other than the one containing a number, returns `f32::NaN`.
    pub fn get_current(&mut self) -> f32 {
        let mut buf = [0u8];
        let mut rtn = String::new();
        loop {
            self.port.read_exact(&mut buf).unwrap();
            if buf[0] == '\n' as u8 {
                break;
            }
            if buf[0] != '\r' as u8 {
                rtn.push(buf[0] as char);
            }
        }
        rtn.parse().unwrap_or(f32::NAN)
    }
}
