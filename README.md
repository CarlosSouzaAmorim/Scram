# Scram
FSM do reator


FSM (Finite State Machine)
Máquina de Estados Finitos (FSM)

PWR (Reator a Água Pressurizada)

SCRAM

stateDiagram-v2
    [*] --> NORMAL
    NORMAL --> HIGH_TEMP: Temperatura > 340°C
    NORMAL --> HIGH_PRESSURE: Pressão > 14.5 MPa
    NORMAL --> LOW_WATER: Nível < 92%
    
    HIGH_TEMP --> NORMAL: Temperatura <= 330°C
    HIGH_TEMP --> SCRAM: Temperatura > 350°C
    
    HIGH_PRESSURE --> NORMAL: Pressão <= 13.5 MPa
    HIGH_PRESSURE --> SCRAM: Pressão > 15.5 MPa
    
    LOW_WATER --> NORMAL: Nível >= 95%
    LOW_WATER --> SCRAM: Nível < 90%
    
    SCRAM --> [*]: Reinício manual

![alt text.](/v5_mermaid.png)


![alt text.](/EBR-I_-_SCRAM_button.jpg)


![alt text.](/2t17fxfaw9af1.webp)

