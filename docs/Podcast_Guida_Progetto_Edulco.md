# Podcast Mini Guide - EdulcoWaterSmart Project

This guide is designed as a short podcast script to include in the repository.
Goal: explain in a simple and practical way how to start, understand, and test the project step by step.

## Recommended Duration

15-20 minutes.

## Audience

- Developers opening the project for the first time
- Technicians who need quick ESP32 tests
- Anyone integrating HTTP commands into Home Assistant

## Episode Outline

## 1) Intro (1 minute)

Suggested text:
Hello, in this episode we will see how to use EdulcoWaterSmart in a practical way.
We start from the repository structure, upload the example to ESP32, and then test HTTP smart commands.
By the end, you will know how to safely add a new command.

## 2) Repository Overview (2 minutes)

Explain these points:
- The main library is in the src folder and contains sensor/control logic.
- Examples are in the examples folder.
- For the web/Home Assistant demo, use examples/EdulcoHomeAssistant/EdulcoHomeAssistant.ino.
- In docs you can find this guide and supporting material.

## 3) Quick Hardware and Firmware Setup (3 minutes)

Steps to explain:
1. Connect ESP32 to your PC.
2. Open examples/EdulcoHomeAssistant/EdulcoHomeAssistant.ino in Arduino IDE.
3. Select the correct ESP32 board and serial port.
4. Set Wi-Fi SSID and password in the example file.
5. Compile and upload.
6. Open Serial Monitor at 115200 to verify startup and assigned IP.

## 4) What the Example Does (2 minutes)

Simple explanation:
- Initializes EdulcoWaterSmart.
- Connects ESP32 to Wi-Fi.
- Starts an HTTP server on port 80.
- Exposes POST endpoint /smartcommand.
- Prints sensor values periodically to Serial.

## 5) Step-by-Step HTTP Command Testing (4 minutes)

Use a PC on the same network as the module.
Replace IP with the one shown in Serial Monitor.

Basic commands:
- curl -X POST http://IP/smartcommand -d "cmd=getdeviceabout"
- curl -X POST http://IP/smartcommand -d "cmd=getcontrolstatus&ctrl=0"
- curl -X POST http://IP/smartcommand -d "cmd=setrelay&relay=1&status=true"
- curl -X POST http://IP/smartcommand -d "cmd=setrelay&relay=1&status=false"

Recommended error checks:
- unknown cmd
- relay out of range
- invalid status

## 6) How to Add a New Command (4 minutes)

Recommended workflow:
1. Open function WSER_handleSmartCommand.
2. Add a new else if branch with your cmd name.
3. Validate all input parameters.
4. Execute action using existing g_device APIs.
5. Return HTTP 200 on success or 400 on error.
6. Test with curl and verify invalid cases too.

Practical rules:
- Keep commands simple and readable.
- Do not add unnecessary dependencies in first release.
- Avoid complex logic in the HTTP parser; delegate to the library when possible.

## 7) Episode Closing (1 minute)

Suggested text:
In this guide we covered repository structure, firmware upload, API testing, and how to add new commands.
In the next episode we can cover endpoint authentication, advanced logging, and full Home Assistant automation integration.

## Final Checklist Before Publishing

- Clear title
- Episode duration
- Minimal prerequisites
- Numbered steps
- Verification commands
- Common errors
- Next steps

End of guide.
