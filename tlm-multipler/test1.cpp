
// Buffer
#include <systemc>
#include <tlm.h>
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>

class Buffer : public sc_core::sc_module {
public:
    tlm_utils::simple_initiator_socket<Buffer> socket;
    SC_HAS_PROCESS(Buffer);

    Buffer(sc_core::sc_module_name name) : socket("socket") {
        SC_THREAD(process);
    }

    void process() {
        int data[64];
        for (int i = 0; i < 64; i += 2) {
            data[i] = i;  // Or other data generation logic
            data[i+1] = i+1;  // Or  other data generation logic
            tlm::tlm_generic_payload* trans = new tlm::tlm_generic_payload;
            trans->set_data_ptr(reinterpret_cast<unsigned char*>(&data[i]));
            trans->set_data_length(2 * sizeof(int));
            trans->set_streaming_width(2 * sizeof(int));
            trans->set_byte_enable_ptr(0);
            trans->set_dmi_allowed(false);
            trans->set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);

             // Send transaction to target
                sc_core :: sc_time delay = sc_core::SC_ZERO_TIME;
                socket->b_transport(*trans, delay);

                // Error checking
              /*  if (trans.is_response_error()) {
                    SC_REPORT_ERROR("TLM-2", "Response error from b_transport");
                }*/

                wait(10, sc_core :: SC_NS);
           
        }
    }
};

// Multiplication Unit
class Multiplier : public sc_core::sc_module {
public:
    tlm_utils::simple_target_socket<Multiplier> socket;
    SC_HAS_PROCESS(Multiplier);

    Multiplier(sc_core::sc_module_name name) : socket("socket") {
        socket.register_b_transport(this, &Multiplier::b_transport);
    }

    void b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& delay) {
        auto* ptr = reinterpret_cast<int*>(trans.get_data_ptr());

        // Perform the multiplication operation
        int result = ptr[0] * ptr[1];

        std::cout << "Multiplication result for unit " << name() << ": " << result << std::endl;

        // Perform the accumulation operation
        static int sum = 0;
        sum += result;

        std::cout << "Accumulation result for unit " << name() << ": " << sum << std::endl;

        // Write the result back to the payload
        *ptr = sum;

        // Update the transaction response status
        trans.set_response_status(tlm::TLM_OK_RESPONSE);
    }
};

// MAC
class MAC : public sc_core::sc_module {
public:
    Buffer* buffers[4];
    Multiplier* multipliers[4];

    MAC(sc_core::sc_module_name name) {
        for (int i = 0; i < 4; i++) {
            buffers[i] = new Buffer(("buffer" + std::to_string(i)).c_str());
            multipliers[i] = new Multiplier(("multiplier" + std::to_string(i)).c_str());
            buffers[i]->socket.bind(multipliers[i]->socket);
        }
    }

    ~MAC() {
        for (int i = 0; i < 4; i++) {
            delete buffers[i];
            delete multipliers[i];
        }
    }
};

int sc_main(int argc, char* argv[]) {
    MAC mac("mac");
    // Start the simulation
    sc_core::sc_start();
    return 0;
}