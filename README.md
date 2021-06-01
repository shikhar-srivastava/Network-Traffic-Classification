# Network-Traffic-Classification
[Shikhar S](https://github.com/shikhar-srivastava),
[Jawahar Reddy](https://github.com/jawahar37)


**Summary:** 
> Simulates a diverse network traffic (implements SSH, P2P, TCP protocols with various features like Multi-threading, Session Tracking, multi-user/client file sharing, multimedia transfer, etc) with DoS/IP-spoofing attacks. (See folders [`network-simulation`](https://github.com/shikhar-srivastava/Network-Traffic-Classification/tree/master/network/network_simulators), [`attack-simulation`](https://github.com/shikhar-srivastava/Network-Traffic-Classification/tree/master/network/attack_simulator)). Uses traffic packet-flow features to analyze network traffic (classify, detect anomalies/attacks, cluster/visualize)

## Contributions
Simulates a real-time consumer-grade network traffic scenario by implementing a multitude of application &amp; network level protocols in C/C++, as well network security attacks (like DoS attacks). The following protocols were implemented purely in C/C++:
- SSH (Secure Socket Shell) Traffic
   - Implemented Session tracking, RSA Key verification & Passkey-Auth
   - SSH Server uses multi-threading to handle simulatenous client requests
- P2P (Peer-to-Peer) for File Transfer
   - Files are segmented and shared amongst multiple users by dynamically switching between server/client functions.
   - Implemented broadcasting to send files to multiple users, multithreading to receive files segments from multiple peers
- Denial-of-Service (DoS) Attack, IP Spoofing
   - Implemented IP Spoofing with psuedo packet-headers
   - Extendable to Distributed DoS (DDos) attack with basic multithreading
- TCP Multimedia File transfer

Classifying network traffic accurately, efficiently in real-time, using features obtained only through the initial packet-flow, and without identifying the protocol-ports to generalize to dynamic &amp; unpredictable traffic.
   - Packet-flow features were generated retroactively on packets collected from hours of network simulation
   - Visualizations of clusters using Dimensionality Reduction & Manifold Learning
   - K-Means Clustering for _Anomaly Detection_ and Random Forest model for traffic classification to safe[TCP,SSH,UDP,others], unsafe [network attacks (Dos), anomolous users] classes.



## Usage
Refer to folders [`network-simulation`](https://github.com/shikhar-srivastava/Network-Traffic-Classification/tree/master/network/network_simulators), [`attack-simulation`](https://github.com/shikhar-srivastava/Network-Traffic-Classification/tree/master/network/attack_simulator)) for files to generate packets from respective network-level/application-level protocols. You can use a packet capture application (see [`wireshark`](https://www.wireshark.org)) to view the generated traffic.
