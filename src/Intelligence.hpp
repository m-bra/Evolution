#ifndef INTELLIGENCE_HPP_INCLUDED
#define INTELLIGENCE_HPP_INCLUDED

#include <nnlib.hpp>
#include <cstdlib>

class Intelligence
{
private:
    nn_network network;
public:
    Intelligence(unsigned layerCount, unsigned *neuronCounts)
    {
        network.init(layerCount, neuronCounts);
    }

    ~Intelligence()
    {
        network.free();
    }

    void calculate(nn_real const *in, nn_real *out)
    {
        network.setinputs(in);
        network.calc_out();
        network.getoutputs(out);
    }

    void modify(float rate = .2f)
    {
        nn_network_code code;
        code.init(&network);

        unsigned charCount = sizeof(*code.values) * code.values_cnt;
        char *chars = (char *) code.values;
        for (int c = 0; c < charCount; ++c)
            for (int b = 0; b < 8; ++b)
                if ((rand() % 1000) / 1000. < rate)
                    // flip bit
                    chars[c]^= 1 << b;

        network.fromcode(&code);
        code.free();
    }
};

#endif/*INTELLIGENCE_HPP_INCLUDED*/
