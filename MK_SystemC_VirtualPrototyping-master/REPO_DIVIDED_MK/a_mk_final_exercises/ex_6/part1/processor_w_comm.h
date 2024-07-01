#ifndef PROCESSOR_H
#define PROCESSOR_H
/* MK_GPT General Explanation:
This code defines a SystemC module called processor that simulates a processor interacting with a memory subsystem using Transaction-Level Modeling (TLM). 
The processor module reads transactions from a trace file or generates random transactions and sends them to the memory subsystem. The transactions include
read and write operations along with their corresponding addresses and data. The processor module implements TLM interfaces for initiator sockets to communicate
with the memory subsystem. Additionally, it includes error handling for file opening and syntax errors in the trace file. The processTrace method parses the
trace file line by line, extracts transaction details, and performs TLM transactions. The processRandom method generates random transactions and performs TLM
transactions based on random cycles and addresses. Finally, the code includes support for backward non-blocking transport and invalidating direct memory pointers.
*/
#include <string> // MK_GPT: Include the string header for string manipulation
#include <iostream> // MK_GPT: Include the iostream header for input/output operations
#include <regex> // MK_GPT: Include the regex header for regular expressions
#include <string> // MK_GPT: Include the string header again (redundant)
#include <iomanip> // MK_GPT: Include the iomanip header for manipulating input/output formatting
#include <random> // MK_GPT: Include the random header for random number generation

#include <systemc.h> // MK_GPT: Include the SystemC header
#include <tlm.h> // MK_GPT: Include the TLM header
#include <tlm_utils/tlm_quantumkeeper.h> // MK_GPT: Include the TLM Quantum Keeper header for time management


class processor : public sc_module, tlm::tlm_bw_transport_if<> // MK_GPT: Declare the processor class inheriting from sc_module and tlm_bw_transport_if
{
private:
    std::ifstream file; // MK_GPT: Declare an input file stream object
    sc_time cycleTime; // MK_GPT: Declare a variable to store the cycle time

    // Methods:
    void processTrace(); // MK_GPT: Declare a method to process a trace file
    void processRandom(); // MK_GPT: Declare a method to generate random transactions

public:
    tlm::tlm_initiator_socket<> iSocket; // MK_GPT: Declare an initiator socket for TLM communication

    processor(sc_module_name, std::string pathToTrace, sc_time cycleTime); // MK_GPT: Declare the constructor for the processor class

    SC_HAS_PROCESS(processor); // MK_GPT: Declare that the processor class has a SystemC process

    // Dummy method:
    void invalidate_direct_mem_ptr(sc_dt::uint64, sc_dt::uint64) // MK_GPT: Declare a dummy method for invalidating direct memory pointers
    {
        SC_REPORT_FATAL(this->name(), "invalidate_direct_mem_ptr not implement"); // MK_GPT: Report a fatal error if the method is called
    }

    // Dummy method:
    tlm::tlm_sync_enum nb_transport_bw(tlm::tlm_generic_payload &,tlm::tlm_phase &, sc_time &) // MK_GPT: Declare a dummy method for backward non-blocking transport
    {
        SC_REPORT_FATAL(this->name(), "nb_transport_bw is not implemented"); // MK_GPT: Report a fatal error if the method is called
        return tlm::TLM_ACCEPTED; // MK_GPT: Return an accepted status
    }
};

processor::processor(sc_module_name, std::string pathToFile, sc_time cycleTime) : // MK_GPT: Define the constructor for the processor class
    file(pathToFile), cycleTime(cycleTime) // MK_GPT: Initialize the file stream object and cycleTime variable
{
    if (!file.is_open()) // MK_GPT: Check if the file is open
        SC_REPORT_FATAL(name(), "Could not open trace"); // MK_GPT: Report a fatal error if the file cannot be opened

    SC_THREAD(processTrace); // MK_GPT: Create a SystemC thread for processing trace
    iSocket.bind(*this); // MK_GPT: Bind the initiator socket to this module
}

// Use the command below in a termial to get your gcc version.
// $ gcc --version
// If it is less than 4.9 uncomment the definition that follows.
// #define GCC_LESS_THAN_4_9_DOES_NOT_SUPPORT_REGEX

void processor::processTrace() // MK_GPT: Define the method to process a trace file
{
    wait(SC_ZERO_TIME); // MK_GPT: Wait for zero time before starting

    std::string line; // MK_GPT: Declare a string variable to store each line of the trace file
    tlm::tlm_generic_payload trans; // MK_GPT: Declare a TLM generic payload object for transactions
    uint64_t cycles = 0; // MK_GPT: Initialize a variable to store cycles
    uint64_t address = 0; // MK_GPT: Initialize a variable to store addresses
    std::string dataStr; // MK_GPT: Declare a string variable to store data in string format
    unsigned char data[4]; // MK_GPT: Declare an array to store data as bytes
    bool read = true; // MK_GPT: Initialize a flag to indicate read or write operation

    while (std::getline(file, line)) // MK_GPT: Read each line from the trace file
    {
#ifdef GCC_LESS_THAN_4_9_DOES_NOT_SUPPORT_REGEX
        // Available GCC is too old and it does not have support to regular
        // expressions (version <= 4.9).
        std::string time; // MK_GPT: Declare a string variable to store the timestamp
        std::string command; // MK_GPT: Declare a string variable to store the command (read or write)
        std::string addr; // MK_GPT: Declare a string variable to store the address
        std::istringstream iss(line); // MK_GPT: Create a string stream to parse the line
        // Get the timestamp for the transaction.
        iss >> time; // MK_GPT: Read the timestamp
        if (time.empty()) { // MK_GPT: Check if timestamp is empty
            SC_REPORT_FATAL(name(), "Syntax error in trace"); // MK_GPT: Report a fatal error if timestamp is empty
        }
        cycles = std::stoull(time.c_str(), nullptr, 10); // MK_GPT: Convert timestamp to an integer

        // Get the command.
        iss >> command; // MK_GPT: Read the command
        if (command.empty()) { // MK_GPT: Check if command is empty
            SC_REPORT_FATAL(name(), "Syntax error in trace"); // MK_GPT: Report a fatal error if command is empty
        }
        if (command == "read") { // MK_GPT: Check if command is read
            read = true; // MK_GPT: Set the read flag to true
        } else if (command == "write") { // MK_GPT: Check if command is write
            read = false; // MK_GPT: Set the read flag to false
        } else { // MK_GPT: If command is neither read nor write
            SC_REPORT_FATAL(name(), "Syntax error in trace"); // MK_GPT: Report a fatal error
        }

        // Get the address.
        iss >> addr; // MK_GPT: Read the address
        if (addr.empty()) { // MK_GPT: Check if address is empty
            SC_REPORT_FATAL(name(), "Syntax error in trace"); // MK_GPT: Report a fatal error if address is empty
        }
        address = std::stoull(addr.c_str(), nullptr, 16); // MK_GPT: Convert address to an integer

        // Get the data if transaction is a write.
        if (read == false) { // MK_GPT: If it is a write operation
            iss >> dataStr; // MK_GPT: Read the data
            if (dataStr.empty()) { // MK_GPT: Check if data is empty
                SC_REPORT_FATAL(name(), "Syntax error in trace"); // MK_GPT: Report a fatal error if data is empty
            }
            unsigned long long d = std::stoull(dataStr.c_str(), nullptr, 16); // MK_GPT: Convert data to an integer
            data[0] = (unsigned char)((d & 0xff000000) >> 24); // MK_GPT: Extract the bytes of data
            data[1] = (unsigned char)((d & 0x00ff0000) >> 16);
            data[2] = (unsigned char)((d & 0x0000ff00) >> 8);
            data[3] = (unsigned char)((d & 0x000000ff) >> 0);
        }

#else
        // Available GCC has support to regular expressions (version >= 4.9)
        std::regex reW("(\\d+)\\s*:\\s*write\\s+0x([\\d\\w]+)\\s+0x([\\d\\w]+)"); // MK_GPT: Define a regex pattern for write transactions
        std::regex reR("(\\d+)\\s*:\\s*read\\s+0x([\\d\\w]+)"); // MK_GPT: Define a regex pattern for read transactions
        std::smatch matchW; // MK_GPT: Declare a match object for write transactions
        std::smatch matchR; // MK_GPT: Declare a match object for read transactions

        if (std::regex_search(line, matchW, reW) && matchW.size() > 1) // MK_GPT: Check if line matches write pattern
        {
            // Extract transaction details
            cycles = std::stoull(matchW.str(1), nullptr, 10); // MK_GPT: Convert timestamp to an integer
            read = false; // MK_GPT: Set the read flag to false
            address = std::stoull(matchW.str(2), nullptr, 16); // MK_GPT: Convert address to an integer
            dataStr = matchW.str(3); // MK_GPT: Extract data as a string
            // Convert data string to bytes
            for(int i = 0; i < 4; i++) // MK_GPT: Loop through each byte
            {
                data[i] = (unsigned char) std::stoi(dataStr.substr(i * 2, 2), // MK_GPT: Convert substring to integer
                        nullptr,
                        16);
            }
        }
        else if (std::regex_search(line, matchR, reR) && matchR.size() > 1) // MK_GPT: Check if line matches read pattern
        {
            // Extract transaction details
            cycles = std::stoull(matchR.str(1), nullptr, 10); // MK_GPT: Convert timestamp to an integer
            read = true; // MK_GPT: Set the read flag to true
            address = std::stoull(matchR.str(2), nullptr, 16); // MK_GPT: Convert address to an integer
        }
        else // MK_GPT: If line does not match any pattern
        {
            SC_REPORT_FATAL(name(), "Syntax error in trace"); // MK_GPT: Report a fatal error
        }
#endif /* GCC_LESS_THAN_4_9_DOES_NOT_SUPPORT_REGEX */

        sc_time delay; // MK_GPT: Declare a variable to store delay

        if (sc_time_stamp() <= cycles * cycleTime) // MK_GPT: Check if current time is less than transaction time
        {
            delay = cycles * cycleTime - sc_time_stamp(); // MK_GPT: Calculate delay
        }
        else // MK_GPT: If current time is greater than transaction time
        {
            delay = SC_ZERO_TIME; // MK_GPT: Set delay to zero
        }

        // Configure transaction payload
        trans.set_address(address); // MK_GPT: Set transaction address
        trans.set_data_length(4); // MK_GPT: Set data length
        trans.set_command(read ? tlm::TLM_READ_COMMAND : tlm::TLM_WRITE_COMMAND); // MK_GPT: Set read/write command
        trans.set_data_ptr(data); // MK_GPT: Set data pointer

        // Perform TLM transaction
        iSocket->b_transport(trans, delay); // MK_GPT: Perform transaction with delay

        wait(delay); // MK_GPT: Wait for the delay

#if 1
        // Print transaction details
        std::cout << std::setfill(' ') << std::setw(4)
            << name() << " "
            << std::setfill(' ') << std::setw(10)
            << sc_time_stamp() << " "
            << std::setfill(' ') << std::setw(5)
            << (read ? "read" : "write") << " 0x"
            << std::setfill('0') << std::setw(8)
            << address
            << " 0x" << std::hex
            << std::setfill('0') << std::setw(2)
            << (unsigned int)data[0]
            << std::setfill('0') << std::setw(2)
            << (unsigned int)data[1]
            << std::setfill('0') << std::setw(2)
            << (unsigned int)data[2]
            << std::setfill('0') << std::setw(2)
            << (unsigned int)data[3]
            << std::endl; // MK_GPT: Print transaction details
#endif
    }

    // End Simulation because there are no events.
}

void processor::processRandom() // MK_GPT: Define the method to generate random transactions
{
    wait(SC_ZERO_TIME); // MK_GPT: Wait for zero time before starting

    tlm::tlm_generic_payload trans; // MK_GPT: Declare a TLM generic payload object for transactions

    uint64_t cycles; // MK_GPT: Declare a variable to store cycles
    uint64_t address; // MK_GPT: Declare a variable to store addresses
    unsigned char data[4]; // MK_GPT: Declare an array to store data as bytes

    std::default_random_engine randGenerator; // MK_GPT: Declare a random number generator engine
    std::uniform_int_distribution<uint64_t> distrCycle(0, 99); // MK_GPT: Declare a uniform distribution for cycles
    std::uniform_int_distribution<uint64_t> distrAddr(0, 1023); // MK_GPT: Declare a uniform distribution for addresses

    data[0] = 0; // MK_GPT: Initialize data to zeros
    data[1] = 0;
    data[2] = 0;
    data[3] = 0;

    trans.set_data_length(4); // MK_GPT: Set data length for transaction
    trans.set_command(tlm::TLM_WRITE_COMMAND); // MK_GPT: Set command for transaction
    trans.set_data_ptr(data); // MK_GPT: Set data pointer for transaction

    for (uint64_t transId = 0; transId < 100000000; transId++) // MK_GPT: Loop for a large number of transactions
    {
        cycles = distrCycle(randGenerator); // MK_GPT: Generate random cycle
        address = distrAddr(randGenerator); // MK_GPT: Generate random address

        sc_time delay; // MK_GPT: Declare a variable to store delay

        if (sc_time_stamp() <= cycles * cycleTime) // MK_GPT: Check if current time is less than transaction time
        {
            delay = cycles * cycleTime - sc_time_stamp(); // MK_GPT: Calculate delay
        }
        else // MK_GPT: If current time is greater than transaction time
        {
            delay = SC_ZERO_TIME; // MK_GPT: Set delay to zero
        }

        // Configure transaction payload
        trans.set_address(address); // MK_GPT: Set transaction address

        // Perform TLM transaction
        iSocket->b_transport(trans, delay); // MK_GPT: Perform transaction with delay

        wait(delay); // MK_GPT: Wait for the delay
    }

    // End Simulation because there are no events.
}

#endif // PROCESSOR_H
