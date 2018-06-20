# Network-Traffic-Classification

_The goal of this project is two fold:_ 
    
  - _To simulate a real-time consumer-grade network traffic scenario by implementing a multitude of application &amp; network level protocols in C/C++, as well network security attacks (like DoS attacks)._ The following protocols were implemented purely in C/C++:
      - SSH (Secure Socket Shell) Traffic [Server & Client side]
          - Implemented Session tracking, RSA Key verification & Passkey-Auth
          - SSH Server uses multi-threading to handle simulatenous client requests
      - P2P (Peer-to-Peer) for File Transfer
           - Files are segmented and shared amongst multiple users by dynamically switching between server/client functions.
           - Implemented broadcasting to send files to multiple users, multithreading to receive files segments from multiple peers
      - Denial-of-Service (DoS) Attack
           - Implemented IP Spoofing with psuedo packet-headers
           - Extendable to Distributed DoS (DDos) attack with basic multithreading 
      - TCP Multimedia File transfer [Server & Client side]
      
  - _Classifying network traffic accurately, efficiently in real-time, using features obtained only through the initial packet-flow, and without identifying the protocol-ports to generalize to dynamic &amp; unpredictable traffic._
     -  Packet-flow features were generated retroactively on packets collected from hours of network simulation
     -  Visualizations of clusters using Dimensionality Reduction & Manifold Learning
     -  K-Means Clustering for _Anomaly Detection_ and Random Forest model for traffic classification to safe[TCP,SSH,UDP,others], unsafe [network attacks (Dos), anomolous users] classes.
