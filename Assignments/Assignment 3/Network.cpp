#include "Network.h"

Network::Network() {

}
std::string activityTypeToString(ActivityType type) {
    switch (type) {
        case ActivityType::MESSAGE_RECEIVED:
            return "Message Received";
        case ActivityType::MESSAGE_FORWARDED:
            return "Message Forwarded";
        case ActivityType::MESSAGE_SENT:
            return "Message Sent";
        case ActivityType::MESSAGE_DROPPED:
            return "Message Dropped";
        default:
            return "Unknown Activity Type";
    }
}
void printCommandHeaderFooter(const std::string& command) {
    std::string separator(command.length() + 9, '-');
    std::cout << separator << "\n";
    std::cout << "Command: " << command << "\n";
    std::cout << separator << "\n";
}
void Network::process_commands(vector<Client> &clients, vector<string> &commands, int message_limit,
                      const string &sender_port, const string &receiver_port) {
    for (const auto& command : commands) {
        std::istringstream iss(command);
        std::string cmd_type;
        iss >> cmd_type;
        if (cmd_type == "MESSAGE") {
            printCommandHeaderFooter(command);
            std::cout<<"Message to be sent: ";
            std::string senderId, receiverId;
            std::string message;
            iss >> senderId >> receiverId;
            size_t start = command.find('#');
            size_t end = command.rfind('#');
            message = command.substr(start + 1, end - start - 1);
            std::cout<<"\""<<message<<"\"\n"<<std::endl;
            Client* receiver;
            int frameNum = 1;
            for (auto& client: clients) {
                if (client.client_id == receiverId) {
                    receiver = &client;
                }
            }
            if (message.length() > message_limit) {
                std::vector<std::string> messageChunks;
                for (size_t i = 0; i < message.length(); i += message_limit) {
                    messageChunks.push_back(message.substr(i, message_limit));

                }
                for (auto& client : clients) {
                    if (client.client_id == senderId) {
                        for (const auto &chunk: messageChunks) {
                            auto *applicationLayerPacket = new ApplicationLayerPacket(0, senderId, receiverId, chunk);
                            auto *transportLayerPacket = new TransportLayerPacket(1, sender_port, receiver_port);
                            auto *networkLayerPacket = new NetworkLayerPacket(2, client.client_ip, receiver->client_ip);
                            auto *physicalLayerPacket = new PhysicalLayerPacket(3, client.client_mac,receiver->client_mac);
                            applicationLayerPacket->frameNumber=frameNum;
                            std::stack<Packet *> packetStack;
                            packetStack.push(applicationLayerPacket);
                            packetStack.push(transportLayerPacket);
                            packetStack.push(networkLayerPacket);
                            packetStack.push(physicalLayerPacket);
                            client.outgoing_queue.push(packetStack);
                            std::string hopId;
                            for (const auto& entry : client.routing_table){
                                if (entry.first==applicationLayerPacket->receiver_ID){
                                    hopId=entry.second;
                                }
                            }Client* hopClient = nullptr;
                            for(auto& client1: clients){
                                if(client1.client_id==hopId){
                                    hopClient=&client1;
                                    break;
                                }
                            }
                            std::cout<<"Frame #"<<applicationLayerPacket->frameNumber<<"\nSender MAC address: "
                            <<physicalLayerPacket->sender_MAC_address<<", Receiver MAC address: "<<hopClient->client_mac
                            <<"\nSender IP address: "<<networkLayerPacket->sender_IP_address<<", Receiver IP address: "
                            <<networkLayerPacket->receiver_IP_address<<"\nSender port number: "<<transportLayerPacket->sender_port_number
                            <<", Receiver port number: "<<transportLayerPacket->receiver_port_number<<"\nSender ID: "
                            <<applicationLayerPacket->sender_ID<<", Receiver ID: "<<applicationLayerPacket->receiver_ID
                            <<"\nMessage chunk carried: \""<<chunk<<"\"\nNumber of hops so far: "<<applicationLayerPacket->hopCount<<"\n--------"<<std::endl;
                            frameNum++;
                        }
                    }
                }
            } else {
                for (auto& client : clients) {
                    if (client.client_id == senderId) {
                        auto* applicationLayerPacket = new ApplicationLayerPacket(0, senderId, receiverId, message);
                        auto* transportLayerPacket = new TransportLayerPacket(1, sender_port, receiver_port);
                        auto* networkLayerPacket = new NetworkLayerPacket(2, client.client_ip, receiver->client_ip);
                        auto* physicalLayerPacket = new PhysicalLayerPacket(3, client.client_mac, receiver->client_mac);
                        applicationLayerPacket->frameNumber=1;
                        std::stack<Packet*> packetStack;
                        packetStack.push(applicationLayerPacket);
                        packetStack.push(transportLayerPacket);
                        packetStack.push(networkLayerPacket);
                        packetStack.push(physicalLayerPacket);
                        client.outgoing_queue.push(packetStack);
                        std::string hopId;
                        for (const auto& entry : client.routing_table){
                            if (entry.first==applicationLayerPacket->receiver_ID){
                                hopId=entry.second;
                            }
                        }Client* hopClient = nullptr;
                        for(auto& client1: clients){
                            if(client1.client_id==hopId){
                                hopClient=&client1;
                                break;
                            }
                        }
                        std::cout<<"Frame #"<<applicationLayerPacket->frameNumber<<"\nSender MAC address: "
                                 <<physicalLayerPacket->sender_MAC_address<<", Receiver MAC address: "<<hopClient->client_mac
                                 <<"\nSender IP address: "<<networkLayerPacket->sender_IP_address<<", Receiver IP address: "
                                 <<networkLayerPacket->receiver_IP_address<<"\nSender port number: "<<transportLayerPacket->sender_port_number
                                 <<", Receiver port number: "<<transportLayerPacket->receiver_port_number<<"\nSender ID: "
                                 <<applicationLayerPacket->sender_ID<<", Receiver ID: "<<applicationLayerPacket->receiver_ID
                                 <<"\nMessage chunk carried: \""<<message<<"\"\nNumber of hops so far: "<<applicationLayerPacket->hopCount<<"\n--------"<<std::endl;
                        break;
                    }
                }
            }

        } else if (cmd_type == "SHOW_FRAME_INFO") {
            printCommandHeaderFooter(command);
            std::string clientId, queueSelection, queueText;
            int frame_count;
            iss >> clientId >> queueSelection >> frame_count;
            frame_count-=1;
            for (auto& client: clients) {
                if (client.client_id == clientId) {
                    std::queue<std::stack<Packet *>> tempQueue;
                    if (queueSelection == "out") {
                        queueText="outgoing";
                        tempQueue = client.outgoing_queue;
                        while (frame_count > 0 && !tempQueue.empty()) {
                            tempQueue.pop();
                            frame_count--;
                        }
                    }else if (queueSelection == "in") {
                        queueText="incoming";
                        tempQueue = client.incoming_queue;
                        while (frame_count > 0 && !tempQueue.empty()) {
                            tempQueue.pop();
                            frame_count--;
                        }
                    }
                    if (!tempQueue.empty()) {
                        std::stack<Packet *> frame = tempQueue.front();
                        std::stack<Packet *> tempStack = frame;
                        auto* physicalLayerPacket = dynamic_cast<PhysicalLayerPacket*>(tempStack.top());
                        tempStack.pop();
                        auto* networkLayerPacket = dynamic_cast<NetworkLayerPacket*>(tempStack.top());
                        tempStack.pop();
                        auto* transportLayerPacket = dynamic_cast<TransportLayerPacket*>(tempStack.top());
                        tempStack.pop();
                        auto* applicationLayerPacket = dynamic_cast<ApplicationLayerPacket*>(tempStack.top());
                        tempStack.pop();
                        std::string hopId;
                        Client* hopClient = nullptr;
                        if (client.client_id!=applicationLayerPacket->receiver_ID){
                            for (const auto& entry : client.routing_table){
                                if (entry.first==applicationLayerPacket->receiver_ID){
                                    hopId=entry.second;
                                }
                            }
                            for(auto& client1: clients){
                                if(client1.client_id==hopId){
                                    hopClient=&client1;
                                    break;
                                }
                            }
                        }else{
                            hopClient = &client;
                        }
                        std::cout<<"Current Frame #"<<applicationLayerPacket->frameNumber
                        <<" on the "<<queueText<<" queue of client "<<client.client_id<<"\nCarried Message: \""
                        <<applicationLayerPacket->message_data<<"\"\nLayer "<<applicationLayerPacket->layer_ID
                        <<" info: Sender ID: "<<applicationLayerPacket->sender_ID<<", Receiver ID: "
                        <<applicationLayerPacket->receiver_ID<<"\nLayer "<<transportLayerPacket->layer_ID
                        <<" info: Sender port number: "<<transportLayerPacket->sender_port_number
                        <<", Receiver port number: "<<transportLayerPacket->receiver_port_number<<"\nLayer "
                        <<networkLayerPacket->layer_ID<<" info: Sender IP address: "
                        <<networkLayerPacket->sender_IP_address<<", Receiver IP address: "
                        <<networkLayerPacket->receiver_IP_address<<"\nLayer "<<physicalLayerPacket->layer_ID
                        <<" info: Sender MAC address: "<<physicalLayerPacket->sender_MAC_address
                        <<", Receiver MAC address: "<<hopClient->client_mac<<"\nNumber of hops so far: "<<applicationLayerPacket->hopCount<<std::endl;
                    }else{
                        std::cout<<"No such frame."<<std::endl;
                    }
                }
            }
        } else if (cmd_type == "SHOW_Q_INFO") {
            std::string clientId, queueSelection, queueSelectionForPrint;
            int frameCount;
            iss >> clientId >> queueSelection;
            for (auto& client: clients) {
                if (client.client_id == clientId) {
                    if (queueSelection =="out"){
                        queueSelectionForPrint = "Outgoing";
                        frameCount=client.outgoing_queue.size();
                    }else if (queueSelection =="in"){
                        queueSelectionForPrint = "Incoming";
                        frameCount=client.incoming_queue.size();
                    }
                    printCommandHeaderFooter(command);
                    std::cout<<"Client "
                    <<client.client_id<<" "<<queueSelectionForPrint<<" Queue Status\nCurrent total number of frames: "
                    <<frameCount<<std::endl;
                }
            }
        } else if (cmd_type == "SEND") {
            printCommandHeaderFooter(command);
            for (auto& client: clients) {
                std::string message;
                std::queue<std::stack<Packet *>> tempQueue = client.outgoing_queue;
                while(!tempQueue.empty()) {
                    std::stack<Packet *> frame = tempQueue.front();
                    std::stack<Packet *> temp = frame;
                    auto* physicalLayerPacket = dynamic_cast<PhysicalLayerPacket*>(temp.top());
                    temp.pop();
                    auto* networkLayerPacket = dynamic_cast<NetworkLayerPacket*>(temp.top());
                    temp.pop();
                    auto* transportLayerPacket = dynamic_cast<TransportLayerPacket*>(temp.top());
                    temp.pop();
                    auto* applicationLayerPacket = dynamic_cast<ApplicationLayerPacket*>(temp.top());
                    temp.pop();
                    std::string receiver;
                    for (const auto& entry : client.routing_table){
                        if (entry.first==applicationLayerPacket->receiver_ID){
                            receiver=entry.second;
                        }
                    }Client* hopClient;
                    for(auto& client1: clients){
                        if(client1.client_id==receiver){
                            hopClient=&client1;
                        }
                    }
                    hopClient->incoming_queue.push(client.outgoing_queue.front());
                    message.append(applicationLayerPacket->message_data);
                    if (!applicationLayerPacket->message_data.empty() && (applicationLayerPacket->message_data.back() == '.' || applicationLayerPacket->message_data.back() == '?' || applicationLayerPacket->message_data.back() == '!')) {
                        if (client.client_id == applicationLayerPacket->sender_ID){
                            auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
                            std::tm timestamp = *std::localtime(&now);
                            std::ostringstream oss;
                            oss << std::put_time(&timestamp, "%Y-%m-%d %H:%M:%S");
                            Log newLog(oss.str(), message, applicationLayerPacket->frameNumber, applicationLayerPacket->hopCount, applicationLayerPacket->sender_ID,applicationLayerPacket->receiver_ID,true,ActivityType::MESSAGE_SENT);
                            client.log_entries.push_back(newLog);
                        }else{
                            auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
                            std::tm timestamp = *std::localtime(&now);
                            std::ostringstream oss;
                            oss << std::put_time(&timestamp, "%Y-%m-%d %H:%M:%S");
                            Log newLog(oss.str(), message, applicationLayerPacket->frameNumber, applicationLayerPacket->hopCount, applicationLayerPacket->sender_ID,applicationLayerPacket->receiver_ID,true,ActivityType::MESSAGE_FORWARDED);
                            client.log_entries.push_back(newLog);
                        }
                        message.clear();
                    }
                    applicationLayerPacket->hopCount++;
                    std::cout<<"Client "<<client.client_id<<" sending frame #"
                             <<applicationLayerPacket->frameNumber<<" to client "<<receiver<<"\nSender MAC address: "
                             <<physicalLayerPacket->sender_MAC_address<<", Receiver MAC address: "<<hopClient->client_mac
                             <<"\nSender IP address: "<<networkLayerPacket->sender_IP_address<<", Receiver IP address: "
                             <<networkLayerPacket->receiver_IP_address<<"\nSender port number: "<<transportLayerPacket->sender_port_number
                             <<", Receiver port number: "<<transportLayerPacket->receiver_port_number<<"\nSender ID: "<<applicationLayerPacket->sender_ID
                             <<", Receiver ID: "<<applicationLayerPacket->receiver_ID<<"\nMessage chunk carried: \""<<applicationLayerPacket->message_data
                             <<"\"\nNumber of hops so far: "<<applicationLayerPacket->hopCount<<"\n--------"<<std::endl;
                    tempQueue.pop();
                    client.outgoing_queue.pop();
                }
            }
        } else if (cmd_type == "RECEIVE") {
            printCommandHeaderFooter(command);
            for (auto& client:clients){
                std::string message;
                bool parameter = false;
                while (!client.incoming_queue.empty()){
                    std::queue<std::stack<Packet*>> tempQueue = client.incoming_queue;
                    auto* physicalLayerPacket = dynamic_cast<PhysicalLayerPacket*>(tempQueue.front().top());
                    tempQueue.front().pop();
                    auto* networkLayerPacket = dynamic_cast<NetworkLayerPacket*>(tempQueue.front().top());
                    tempQueue.front().pop();
                    auto* transportLayerPacket = dynamic_cast<TransportLayerPacket*>(tempQueue.front().top());
                    tempQueue.front().pop();
                    auto* applicationLayerPacket = dynamic_cast<ApplicationLayerPacket*>(tempQueue.front().top());
                    tempQueue.front().pop();
                    if (client.client_id == applicationLayerPacket->receiver_ID){
                        std::cout<<"Client "<<client.client_id<<" receiving frame #"<<applicationLayerPacket->frameNumber
                                 <<" from client "<<physicalLayerPacket->sender_MAC_address[0]<<", originating from client "
                                 <<applicationLayerPacket->sender_ID<<"\nSender MAC address: "<<physicalLayerPacket->sender_MAC_address
                                 <<", Receiver MAC address: "<<physicalLayerPacket->receiver_MAC_address<<"\nSender IP address: "
                                 <<networkLayerPacket->sender_IP_address<<", Receiver IP address: "<<networkLayerPacket->receiver_IP_address
                                 <<"\nSender port number: "<<transportLayerPacket->sender_port_number<<", Receiver port number: "
                                 <<transportLayerPacket->receiver_port_number<<"\nSender ID: "<<applicationLayerPacket->sender_ID
                                 <<", Receiver ID: "<<applicationLayerPacket->receiver_ID<<"\nMessage chunk carried: \""
                                 <<applicationLayerPacket->message_data<<"\"\nNumber of hops so far: "
                                 <<applicationLayerPacket->hopCount<<"\n--------"<<std::endl;
                        message.append(applicationLayerPacket->message_data);
                        Client *sender;
                        for (auto&client : clients){
                            if (client.client_id==applicationLayerPacket->sender_ID){
                                sender=&client;
                            }
                        }
                        if (!message.empty() && (message.back() == '.' || message.back() == '?' || message.back() == '!')) {
                               auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
                               std::tm timestamp = *std::localtime(&now);
                               std::ostringstream oss;
                               oss << std::put_time(&timestamp, "%Y-%m-%d %H:%M:%S");
                               Log newLog(oss.str(), message, applicationLayerPacket->frameNumber, applicationLayerPacket->hopCount, applicationLayerPacket->sender_ID,applicationLayerPacket->receiver_ID,true,ActivityType::MESSAGE_RECEIVED);
                               client.log_entries.push_back(newLog);
                            std::cout<<"Client "<<client.client_id<<" received the message \""<<message
                            <<"\" from client "<<applicationLayerPacket->sender_ID<<".\n--------"<<std::endl;
                            message.clear();
                        }
                    }
                    else {
                        std::string receiver;
                        for (const auto& entry : client.routing_table){
                            if (entry.first==applicationLayerPacket->receiver_ID){
                                receiver=entry.second;
                            }
                        }Client* hopClient = nullptr;
                        for(auto& client1: clients){
                            if(client1.client_id==receiver){
                                hopClient=&client1;
                                break;
                            }
                        }if (hopClient==nullptr){
                            Client *sender;
                            for (auto&client : clients){
                                if (client.client_id==applicationLayerPacket->sender_ID){
                                    sender=&client;
                                }
                            }
                            std::cout<<"Client "<<client.client_id<<" receiving frame #"<<applicationLayerPacket->frameNumber
                            <<" from client "<<applicationLayerPacket->sender_ID<<", but intended for client "
                            <<applicationLayerPacket->receiver_ID<<". Forwarding... \nError: Unreachable destination. Packets are dropped after "
                            <<applicationLayerPacket->hopCount<<" hops!\n";
                            message.append(applicationLayerPacket->message_data);
                            if (!message.empty() && (message.back() == '.' || message.back() == '?' || message.back() == '!')) {
                                auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
                                std::tm timestamp = *std::localtime(&now);
                                std::ostringstream oss;
                                oss << std::put_time(&timestamp, "%Y-%m-%d %H:%M:%S");
                                Log newLog(oss.str(), message, applicationLayerPacket->frameNumber, applicationLayerPacket->hopCount, applicationLayerPacket->sender_ID,applicationLayerPacket->receiver_ID,false,ActivityType::MESSAGE_DROPPED);
                                client.log_entries.push_back(newLog);
                                std::cout<<"--------"<<std::endl;
                                message.clear();
                            }

                        }else{
                            if (applicationLayerPacket->frameNumber==1){
                                std::cout<<"Client "<<client.client_id<<" receiving a message from client "<<physicalLayerPacket->sender_MAC_address[0]
                                         <<", but intended for client "<<applicationLayerPacket->receiver_ID<<". Forwarding...\n";
                            }
                            std::cout<<"Frame #"<< applicationLayerPacket->frameNumber<<" MAC address change: New sender MAC "
                                     <<client.client_mac<<", new receiver MAC "<<hopClient->client_mac<<std::endl;
                            Client *sender;
                            for (auto&client : clients){
                                if (client.client_id==applicationLayerPacket->sender_ID){
                                    sender=&client;
                                }
                            }
                            if (!applicationLayerPacket->message_data.empty() && (applicationLayerPacket->message_data.back() == '.' || applicationLayerPacket->message_data.back() == '?' || applicationLayerPacket->message_data.back() == '!')) {
                                std::cout<<"--------\n";
                            }
                            physicalLayerPacket->sender_MAC_address = client.client_mac;
                            physicalLayerPacket->receiver_MAC_address = hopClient->client_mac;
                            client.outgoing_queue.push(client.incoming_queue.front());
                            parameter = true;
                        }
                    }

                    client.incoming_queue.pop();
                }
            }
        } else if (cmd_type == "PRINT_LOG") {
            std::string client_id;
            bool parameter = false;
            int i =1;
            iss >> client_id;
            std::string successText;
            printCommandHeaderFooter(command);
            for (auto&client : clients){
                if (client.client_id==client_id){
                    if (!client.log_entries.empty()){
                        std::cout<<"Client "<<client_id<<" Logs:\n--------------\n";
                    }
                    for (Log log:client.log_entries){
                        if (log.success_status){
                            successText = "Yes";
                        }else{
                            successText = "No";
                        }
                        if (parameter){
                            std::cout<<"--------------\n";
                        }
                        std::cout<<"Log Entry #"<<i<<":\nActivity: "<<activityTypeToString(log.activity_type)
                        <<"\nTimestamp: "<<log.timestamp<<"\nNumber of frames: "<<log.number_of_frames<<"\nNumber of hops: "
                        <<log.number_of_hops<<"\nSender ID: "<<log.sender_id<<"\nReceiver ID: "<<log.receiver_id<<"\nSuccess: "
                        <<successText<<std::endl;
                        if (log.activity_type!=ActivityType::MESSAGE_FORWARDED && log.activity_type!=ActivityType::MESSAGE_DROPPED){
                            std::cout<<"Message: \""<<log.message_content<<"\""<<std::endl;
                        }
                        parameter=true;
                        i++;
                    }
                }
            }
        } else {
            printCommandHeaderFooter(command);
            std::cout<<"Invalid command."<<std::endl;
        }
    }


    // TODO: Execute the commands given as a vector of strings while utilizing the remaining arguments.
    /* Don't use any static variables, assume this method will be called over and over during testing.
     Don't forget to update the necessary member variables after processing each command. For example,
     after the MESSAGE command, the outgoing queue of the sender must have the expected frames ready to send. */
}

vector<Client> Network::read_clients(const string &filename) {
    vector<Client> clients;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return clients;
    }
    int numClients;
    file >> numClients;
    for (int i = 0; i < numClients; ++i) {
        std::string id, ip, mac;
        file >> id >> ip >> mac;
        Client client(id, ip, mac);
        clients.emplace_back(client);
    }
    file.close();
    // TODO: Read clients from the given input file and return a vector of Client instances.
    return clients;
}

void Network::read_routing_tables(vector<Client> &clients, const string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }
    int clientIndex = 0;
    std::string line;
    while (std::getline(file, line)) {
        if (line == "-") {
            clientIndex++;
            continue;
        }
        std::unordered_map<std::string, std::string> routingTable;
        std::istringstream iss(line);
        std::string key, value;
        while (iss >> key >> value) {
            routingTable[key] = value;
        }
        if (clientIndex < clients.size()) {
            for (const auto& entry : routingTable) {
                clients[clientIndex].routing_table[entry.first] = entry.second;
            }
        }
    }
    file.close();
    // TODO: Read the routing tables from the given input file and populate the clients' routing_table member variable.
}

vector<string> Network::read_commands(const string &filename) {
    vector<string> commands;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return commands;
    }
    std::string command;
    std::getline(file, command);
    while (std::getline(file, command)) {
        size_t first_non_whitespace = command.find_first_not_of(" \t");
        size_t last_non_whitespace = command.find_last_not_of(" \t");
        if (first_non_whitespace != std::string::npos && last_non_whitespace != std::string::npos) {
            commands.push_back(command.substr(first_non_whitespace, last_non_whitespace - first_non_whitespace + 1));
        }
    }
    file.close();
    // TODO: Read commands from the given input file and return them as a vector of strings.
    return commands;
}

Network::~Network() {
    // TODO: Free any dynamically allocated memory if necessary.
}
