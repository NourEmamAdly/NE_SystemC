#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <string>
#include <iostream>
#include <regex>
#include <string>
#include <iomanip>
#include <random>

#include <systemc.h>
#include <tlm.h>
#include <tlm_utils/tlm_quantumkeeper.h>


class processor : public sc_module, tlm::tlm_bw_transport_if<>
{
private:
	std::ifstream file;
	sc_time cycleTime;
	tlm_utils::tlm_quantumkeeper quantumKeeper;   // MK_Added

	// Method:
    void processTrace();
    void processRandom();

public:
	tlm::tlm_initiator_socket<> iSocket;

	// MK_CHANGED from : processor(sc_module_name, std::string pathToTrace, sc_time cycleTime);
	processor(sc_module_name);

	SC_HAS_PROCESS(processor);

	// Dummy method:
    void invalidate_direct_mem_ptr(sc_dt::uint64, sc_dt::uint64)
	{
		SC_REPORT_FATAL(this->name(), "invalidate_direct_mem_ptr not implement");
	}

	// Dummy method:
    tlm::tlm_sync_enum nb_transport_bw(tlm::tlm_generic_payload &,tlm::tlm_phase &, sc_time &)
	{
		SC_REPORT_FATAL(this->name(), "nb_transport_bw is not implemented");
		return tlm::TLM_ACCEPTED;
	}
};
// MK_CHANGED FOLLOWING FIVE LINES:
//processor::processor(sc_module_name, std::string pathToFile, sc_time cycleTime) :
//	file(pathToFile), cycleTime(cycleTime)
//{
//	if (!file.is_open())
//		SC_REPORT_FATAL(name(), "Could not open trace");
processor::processor(sc_module_name) 
{
    SC_THREAD(processRandom);   			// MK_Changed from : SC_THREAD(processTrace);

	iSocket.bind(*this);

	// MK_ADDED two below
	quantumKeeper.set_global_quantum(sc_time(100,SC_NS)); // STATIC!
    quantumKeeper.reset();
}

// Use the command below in a termial to get your gcc version.
// $ gcc --version
// If it is less than 4.9 uncomment the definition that follows.
// #define GCC_LESS_THAN_4_9_DOES_NOT_SUPPORT_REGEX

void processor::processTrace()
{
	wait(SC_ZERO_TIME);

    std::string line;
	tlm::tlm_generic_payload trans;
	uint64_t cycles = 0;
	uint64_t address = 0;
	std::string dataStr;
	unsigned char data[4];
	bool read = true;

    while (std::getline(file, line))
	{
#ifdef GCC_LESS_THAN_4_9_DOES_NOT_SUPPORT_REGEX
		// Available GCC is too old and it does not have support to regular
		// expressions (version <= 4.9).
		std::string time;
		std::string command;
		std::string addr;
		std::istringstream iss(line);
		// Get the timestamp for the transaction.
		iss >> time;
		if (time.empty()) {
			SC_REPORT_FATAL(name(), "Syntax error in trace");
		}
		cycles = std::stoull(time.c_str(), nullptr, 10);

		// Get the command.
		iss >> command;
		if (command.empty()) {
			SC_REPORT_FATAL(name(), "Syntax error in trace");
		}
		if (command == "read") {
			read = true;
		} else if (command == "write") {
			read = false;
		} else {
			SC_REPORT_FATAL(name(), "Syntax error in trace");
		}

		// Get the address.
		iss >> addr;
		if (addr.empty()) {
			SC_REPORT_FATAL(name(), "Syntax error in trace");
		}
		address = std::stoull(addr.c_str(), nullptr, 16);

		// Get the data if transaction is a write.
		if (read == false) {
			iss >> dataStr;
			if (dataStr.empty()) {
				SC_REPORT_FATAL(name(), "Syntax error in trace");
			}
			unsigned long long d = std::stoull(dataStr.c_str(), nullptr, 16);
			data[0] = (unsigned char)((d & 0xff000000) >> 24);
			data[1] = (unsigned char)((d & 0x00ff0000) >> 16);
			data[2] = (unsigned char)((d & 0x0000ff00) >> 8);
			data[3] = (unsigned char)((d & 0x000000ff) >> 0);
		}

#else
		// Available GCC has support to regular expressions (version >= 4.9)
		std::regex reW("(\\d+)\\s*:\\s*write\\s+0x([\\d\\w]+)\\s+0x([\\d\\w]+)");
		std::regex reR("(\\d+)\\s*:\\s*read\\s+0x([\\d\\w]+)");
		std::smatch matchW;
		std::smatch matchR;

		if (std::regex_search(line, matchW, reW) && matchW.size() > 1)
		{
			cycles = std::stoull(matchW.str(1), nullptr, 10);
			read = false;
			address = std::stoull(matchW.str(2), nullptr, 16);
			dataStr = matchW.str(3);
			for(int i = 0; i < 4; i++)
			{
				data[i] = (unsigned char) std::stoi(dataStr.substr(i * 2, 2),
						nullptr,
						16);
			}
		}
		else if (std::regex_search(line, matchR, reR) && matchR.size() > 1)
		{
			cycles = std::stoull(matchR.str(1), nullptr, 10);
			read = true;
			address = std::stoull(matchR.str(2), nullptr, 16);
		}
		else
		{
			SC_REPORT_FATAL(name(), "Syntax error in trace");
		}
#endif /* GCC_LESS_THAN_4_9_DOES_NOT_SUPPORT_REGEX */


		sc_time delay;

		if (sc_time_stamp() <= cycles * cycleTime)
		{
			delay = cycles * cycleTime - sc_time_stamp();
		}
		else
		{
            delay = SC_ZERO_TIME;
		}

		trans.set_address(address);
		trans.set_data_length(4);
		trans.set_command(read ? tlm::TLM_READ_COMMAND : tlm::TLM_WRITE_COMMAND);
		trans.set_data_ptr(data);
		iSocket->b_transport(trans, delay);

		wait(delay);

#if 1
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
			<< std::endl;
#endif
	}

	// End Simulation because there are no events.
}

void processor::processRandom()
{
    //wait(SC_ZERO_TIME);  //MK_REMOVED		// MK_GPT:  This line suspends the execution of the process until the time advances to the current simulation time (i.e., it waits for zero time).

    tlm::tlm_generic_payload trans;			//MK_GPT: Declares a TLM generic payload object named trans, which is used to represent transactions in the TLM model.

    uint64_t cycles;
    uint64_t address;
    unsigned char data[4];

    std::default_random_engine randGenerator;
    std::uniform_int_distribution<uint64_t> distrCycle(0, 99);
    std::uniform_int_distribution<uint64_t> distrAddr(0, 1023);
	/*
	MK_GPT:
	std::default_random_engine randGenerator;: Creates a random number engine named randGenerator.
	std::uniform_int_distribution<uint64_t> distrCycle(0, 99);: Defines a uniform distribution named distrCycle for random numbers in the range [0, 99].
	std::uniform_int_distribution<uint64_t> distrAddr(0, 1023);: Defines a uniform distribution named distrAddr for random numbers in the range [0, 1023].
	*/

    data[0] = 0;
    data[1] = 0;
    data[2] = 0;
    data[3] = 0;
    
    trans.set_data_length(4);
    trans.set_command(tlm::TLM_WRITE_COMMAND);
    trans.set_data_ptr(data);

	/*
	MK_GPT:
	Enters a loop to generate and process transactions. The loop will iterate 100,000,000 times.
	Generates random values for cycles and address using the random number distributions.
	*/
    //for (uint64_t transId = 0; transId < 100000000; transId++)
	//sc_time delay = quantumKeeper.get_local_time();
	for (uint64_t transId = 0; transId < 10000; transId++)
    {
        cycles = distrCycle(randGenerator);
        address = distrAddr(randGenerator);

        sc_time delay;
		delay = quantumKeeper.get_local_time();
		cout << this->name() << ": B_TRANSPORT @" << sc_time_stamp()
                 << " Local Time " << quantumKeeper.get_local_time() << endl;
        //if (sc_time_stamp() <= cycles * cycleTime)
        //{
        //    delay = cycles * cycleTime - sc_time_stamp();
        //}
        //else
        //{
        //    delay = quantumKeeper.get_local_time(); //MK_CHANGED from: delay = SC_ZERO_TIME;
        //}
		//sc_time delay = cycles * cycleTime;			//NEWW MK_GPT: Calculates the delay for the transaction based on the number of cycles and the cycle time.

		/*
		MK_GPT:
		Sets the address of the transaction.
		Calls the blocking transport method b_transport on the initiator socket iSocket to send the transaction to the target with the specified delay.
		*/
        trans.set_address(address);
        iSocket->b_transport(trans, delay);
		
		// MK_ADDED two lines below
		quantumKeeper.set(delay); // Anotate the time of the target
        quantumKeeper.inc(sc_time(10,SC_NS)); // Consume computation time

        // MK_REMOVED : wait(delay);			// MK_GPT: Waits for the delay to simulate the processing time of the transaction before proceeding to the next iteration of the loop.

		//MK_ADDED below
		if(quantumKeeper.need_sync())
        {
			cout << this->name() << ": Context Switch @"
                     << sc_time_stamp() << endl;
			quantumKeeper.sync();
        }
    }

    // End Simulation because there are no events.
}

#endif // PROCESSOR_H
