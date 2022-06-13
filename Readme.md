# WeDAQ_Multi

## Goal
- The goal of this repo is to allow multiple wifi synchronized streaming from one router using OpenBCI boards through Lab Streaming Layer (LSL).

## Repo Info
- This repo is a extension of a established repo Brainflow. The link to the repo: [Brainflow](https://github.com/brainflow-dev/brainflow/)

## Initial Changes Made
- The python software library was in .py instead of the notebook format .ipynb
- All the necessary packages and more are included in the BrainflowAnacondaEnv.yaml which is a file export for Anaconda Environment

## Unchain Mode
- Unchain mode is added to the OpenBCI board firmware to reduce/eliminate the prominent noise caused by the OpenBCI Wifi boards.
- This Repo utilize the unchain mode of the new OpenBCI board firmware for Wifi. Please flash the firmware before proceeding to run the code for lsl streaming.
- If you do not wish to use the unchain mode, simply remove or comment out the board.config_board("P") in the later parts of the blsl.ipynb code.

## How to Run the Code for LSL Streaming
Download Anaconda
Upload the BrainflowAnacondaEnv.yaml environment to created Anaconda Environment tailored for this code
Open blsl1.ipynb. Find the correct ip address for the OpenBCI Wifi Board connected to your router and type that ip address to the params.ip_addres = 
For multiple streams, open blsl2.ipynb or copy a new blsl1.ipynb file to how many OpenBCI Wifi Boards you plan to play at once. 
Make sure all boards are connected to the router which your computer is connected to. Type the corresponding ip_address for each blsl ipynb file and make sure the params.ip_port is different for each OpenBCI boards.


## Lab Recorder
The lsl streaming was recorded using [Lab Recorder App](https://github.com/labstreaminglayer/App-LabRecorder/releases).
In addition, the lsl streaming can be visualize real time through [BrainVision LSL Viewer](https://pressrelease.brainproducts.com/lsl-viewer/)

## Analyze Code
The lsl stream recorded using Lab Recorder should be an .xdf file. It can be analyzed using pyxdf_read.ipynb in our repo.
