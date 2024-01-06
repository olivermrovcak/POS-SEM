    //
    // Created by Oliver Mrovcak on 04/01/2024.
    //

    #include "Download.h"

    using boost::asio::ip::tcp;

    Download::Download(std::string protocol, std::string hostname, std::string savePath, std::string downloadPath, std::string username, std::string password, int priority) {
        this->protocol = protocol;
        this->hostname = hostname;
        this->downloadPath = downloadPath;
        this->username = username;
        this->password = password;
        this->priority = priority;
        this->savePath = savePath;
    }

    Download::~Download() {

    }

    const std::string &Download::getUsername() const {
        return username;
    }

    const std::string &Download::getPassword() const {
        return password;
    }

    const std::string &Download::getHostname() const {
        return hostname;
    }

    const std::string &Download::getSavePath() const {
        return savePath;
    }

    const std::string &Download::getDownloadPath() const {
        return downloadPath;
    }

    const std::string &Download::getProtocol() const {
        return protocol;
    }

    unsigned long Download::getSize() {
        return size;
    }

    const std::string &Download::getFilename()  {
        return "file";
    }

    unsigned long Download::getCurrentSize() {
        return currentSize;
    }

    void Download::setSize(unsigned long size) {
        this->size = size;
    }

    void Download::start() {
        if (protocol == "FTP") {
            ftpDownload();
        } else if (protocol == "FTPS") {
            // FTPS related code
        } else if (protocol == "HTTP") {
            // HTTP related code
        } else if (protocol == "HTTPS") {
            // HTTPS related code
        } else {
            std::cout << "Invalid protocol" << std::endl;
        }
    }


    void Download::pause() {
        paused = true;
    }

    void Download::resume() {
        paused = false;
    }

    void Download::cancel() {
        // Cancel download
    }

    void Download::restart() {
        // Restart download
    }

    void Download::setPriority(int priority) {
        this->priority = priority;
    }

    void Download::ftpDownload() {



        boost::asio::io_service io_service;

        // Resolve the server address and port
        tcp::resolver resolver(io_service);
        tcp::resolver::query query(this->hostname, "21");
        tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

        // Create and connect the socket
        tcp::socket socket(io_service);
        boost::asio::connect(socket, endpoint_iterator);

        // Function to read a response line from the server
        auto read_response = [&](tcp::socket &sock) {
            boost::asio::streambuf response;
            boost::asio::read_until(sock, response, "\n");
            std::string s((std::istreambuf_iterator<char>(&response)), std::istreambuf_iterator<char>());
            printw("%s\n", s.c_str()); // Change std::cout to printw
            return s;
        };

        // Read the welcome message
        read_response(socket);

        // Send the USER command
        std::string user_cmd = "USER " + this->username + "\r\n";
        boost::asio::write(socket, boost::asio::buffer(user_cmd));
        read_response(socket);

        // Send the PASS command
        std::string pass_cmd = "PASS " + this->password + "\r\n";
        boost::asio::write(socket, boost::asio::buffer(pass_cmd));
        read_response(socket);

        // Send the PASV command to enter passive mode
        std::string pasv_cmd = "PASV\r\n";
        boost::asio::write(socket, boost::asio::buffer(pasv_cmd));
        auto pasv_response = read_response(socket);

        std::string size_cmd = "SIZE " + this->downloadPath + "\r\n";
        boost::asio::write(socket, boost::asio::buffer(size_cmd));
        auto size_response = read_response(socket);

        // Parse the response to get the size
        std::regex size_regex("\\s(\\d+)");
        std::smatch matches;
        if (std::regex_search(size_response, matches, size_regex) && matches.size() == 2) {
            unsigned long file_size = std::stoul(matches[1].str());
            setSize(file_size);
        } else {
            std::cerr << "Failed to get the file size." << std::endl;
        }

        // Parse the PASV response for IP address and port
        std::string data_ip;
        unsigned short data_port;

        if (parse_pasv_response(pasv_response, data_ip, data_port)) {
            std::cout << "Data connection details - IP: " << data_ip << " Port: " << data_port << std::endl;

            // Connect a new socket for the data transfer
            tcp::endpoint data_endpoint(boost::asio::ip::address::from_string(data_ip), data_port);
            tcp::socket data_socket(io_service);
            data_socket.connect(data_endpoint);

            // Send the RETR command
            std::string retr_cmd = "RETR " + this->downloadPath + "\r\n";
            boost::asio::write(socket, boost::asio::buffer(retr_cmd));
            read_response(socket);

            size_t lastSlashPos = this->downloadPath.find_last_of("/\\");
            std::string filename = lastSlashPos != std::string::npos ? this->downloadPath.substr(lastSlashPos + 1) : this->downloadPath;
            std::string fullSavePath = this->savePath + (this->savePath.back() == '/' ? "" : "/") + filename;

            // Open a file to write the downloaded data
            std::ofstream outfile;

            if (std::ifstream(fullSavePath)) {
                std::cerr << "File already exists. Downloading copy" << std::endl;
                outfile.open(getNewFileName(fullSavePath) , std::ios::binary);
            } else {
                 outfile.open(fullSavePath, std::ios::binary);
            }

            // Read and write the file data from the data socket
            boost::asio::streambuf response;
            boost::system::error_code ec;
            while (boost::asio::read(data_socket, response, boost::asio::transfer_at_least(1), ec)) {
                size_t data_length = response.size(); // Get the length of the received data
                currentSize += data_length; // Update the current download size
                outfile << &response;
            }

            if (ec && ec != boost::asio::error::eof) {
                printw("Error during read: %s\n", ec.message().c_str()); // Change std::cerr to printw for errors
            }

            outfile.close();
            data_socket.close();
        } else {
            std::cerr << "Failed to parse the PASV response." << std::endl;
        }
    }

    bool Download::parse_pasv_response(const std::string &response, std::string &ip_address, unsigned short &port) {
            std::regex pasv_regex("\\((\\d+),(\\d+),(\\d+),(\\d+),(\\d+),(\\d+)\\)");
            std::smatch matches;
            if (std::regex_search(response, matches, pasv_regex) && matches.size() == 7) {
                ip_address = matches[1].str() + "." + matches[2].str() + "." + matches[3].str() + "." + matches[4].str();
                int p1 = std::stoi(matches[5].str());
                int p2 = std::stoi(matches[6].str());
                port = static_cast<unsigned short>((p1 * 256) + p2);
                return true;
            }
            return false;
    }

   std::string Download::getNewFileName(const std::string& originalPath) {
        std::string basePath = originalPath.substr(0, originalPath.find_last_of("."));
        std::string extension = originalPath.substr(originalPath.find_last_of("."));

        int counter = 1; // Start numbering from 1
        std::string newPath = basePath + "(" + std::to_string(counter) + ")" + extension;

        // Keep incrementing the counter until the file doesn't exist
        while (std::ifstream(newPath)) {
            newPath = basePath + "(" + std::to_string(++counter) + ")" + extension;
        }

        return newPath;
    }




