#include <systemc.h>

SC_MODULE(Register) {
    sc_in_clk clk;          // Clock input
    sc_in<bool> rst;        // Reset input
    sc_in<sc_uint<8>> data_in;  // Input data
    sc_out<sc_uint<8>> data_out; // Output data

    sc_signal<sc_uint<8>> reg_data; // Internal register data

    void register_process() {
        // On every rising edge of the clock
        if (rst.read()) { // Reset condition
            reg_data = 0; // Reset the register data
        } else if (clk.posedge()) {
            reg_data = data_in.read(); // Update register data with input data
        }

        // Output the register data
        data_out.write(reg_data);
    }

    SC_CTOR(Register) {
        SC_METHOD(register_process);
        sensitive << clk.pos(); // Sensitivity to positive clock edge
        dont_initialize();      // Ensure the method is not initially triggered
    }
};



SC_MODULE(Testbench) {
    sc_out<bool> rst;        // Reset output
    sc_out<sc_uint<8>> data_in;  // Input data
    sc_in<sc_uint<8>> data_out; // Output data

    void stimulus() {
        // Apply reset for 20 ns
        rst.write(true);
        data_in.write(0);
        cout << "@" << sc_time_stamp() << " Asserting reset" << endl;
        wait(20, SC_NS);
        rst.write(false);
        cout << "@" << sc_time_stamp() << " De-asserting reset" << endl;

        // Apply data for 50 ns
        data_in.write(0xAA);
        cout << "@" << sc_time_stamp() << " Writing data: 0xAA" << endl;
        wait(50, SC_NS);

        // Apply another data for 30 ns
        data_in.write(0x55);
        cout << "@" << sc_time_stamp() << " Writing data: 0x55" << endl;
        wait(30, SC_NS);

        // Apply reset for 10 ns
        rst.write(true);
        cout << "@" << sc_time_stamp() << " Asserting reset" << endl;
        wait(10, SC_NS);
        rst.write(false);
        cout << "@" << sc_time_stamp() << " De-asserting reset" << endl;

        // Wait for 50 ns to observe the final output
        wait(50, SC_NS);

        // End simulation
        sc_stop();
    }

    SC_CTOR(Testbench) {
        SC_THREAD(stimulus);
    }
};

int sc_main(int argc, char* argv[]) {
    sc_clock clk("clk", 10, SC_NS); // Create a clock with a period of 10 ns
    sc_signal<bool> rst;            // Reset signal
    sc_signal<sc_uint<8>> data_in;  // Input data
    sc_signal<sc_uint<8>> data_out; // Output data

    Register reg("reg"); // Instantiate the register module
    reg.clk(clk);        // Connect clock
    reg.rst(rst);        // Connect reset
    reg.data_in(data_in); // Connect input data
    reg.data_out(data_out); // Connect output data

    Testbench tb("tb"); // Instantiate the testbench module
    tb.rst(rst);        // Connect reset
    tb.data_in(data_in); // Connect input data
    tb.data_out(data_out); // Connect output data

    sc_trace_file *wf = sc_create_vcd_trace_file("waveform");
    sc_trace(wf, clk, "clk");
    sc_trace(wf, rst, "rst");
    sc_trace(wf, data_in, "data_in");
    sc_trace(wf, data_out, "data_out");

    sc_start(); // Start simulation

    sc_close_vcd_trace_file(wf);

    return 0;
}
