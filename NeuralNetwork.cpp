#include <iostream>
#include <list>
#include <random>

float sigmoid(float x)
{
    int alpha = 2;
    return 1 / ( 1+exp(-x*alpha));
};

class Neuron
{
    private:
       float *w;
       int input;

    public:
       Neuron(int input = 0);
       float feedforward(float *outputs,float bias);
};

inline Neuron::Neuron(int input)
{
    Neuron::input = input;
    //std::cout << "inputs "<< input << std::endl;

    Neuron::w = new float[input];

    // Initializing weights
    for(int i = 0;i<input;i++)
    {
        Neuron::w[i] = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/1.0));
        //std::cout << " amount of w : "<< w[i] << std::endl;
    };
};

inline float Neuron::feedforward(float* input,float bias)
{
    float result;
    for(int i = 0;i<Neuron::input;i++)
    {
        result += w[i]*input[i];
        //std::cout << w[i] << "*" << input[i] << "="<< sigmoid(w[i]*input[i]) << std::endl;
    }
    return sigmoid(result+bias);
};


class NeuronLayer
{
    private:
       Neuron *neurons;
       int num_neurons;
       int num_inputs;

    public:
       NeuronLayer(int num_neurons = 0,int num_inputs = 1);
       void inf();
       float* feedforward(float *input,float bias);
};

NeuronLayer::NeuronLayer(int num_neurons,int num_inputs)
{
    NeuronLayer::num_neurons = num_neurons;
    NeuronLayer::num_inputs = num_inputs;

    NeuronLayer::neurons = new Neuron[num_neurons];
    //std::cout << "Neuron placholders created!" << std::endl;


    for(int i = 0; i<num_neurons;i++)
    {
        neurons[i] = (Neuron(num_inputs));
    }
    //std::cout << "Neuron layer with "<< num_inputs << " inputs and "<< num_neurons<< " neurons created" << std::endl;
};

// function to print out info about NeuronLayer object
void NeuronLayer::inf()
{
  std::cout << "neuron amount: " << NeuronLayer::num_neurons << std::endl << "number of inputs : " << NeuronLayer::num_inputs << std::endl;
}

inline float* NeuronLayer::feedforward(float* input,float bias)
{
    float *results = new float[NeuronLayer::num_neurons];

    for(int i = 0; i < NeuronLayer::num_neurons; i++)
    {
        results[i] = NeuronLayer::neurons[i].feedforward(input,bias);
    };
    return results;
};


class NeuralNetwork
{
    private:
       float bias;
       int num_inputs;
       int num_outputs;
       int num_layers;

       float* result;

       NeuronLayer *l;

    public:
       NeuralNetwork(int layersInf[],int num_inputs,int num_outputs,int num_layers);
       void inf();
       float* feedforward(float *input);
};

inline NeuralNetwork::NeuralNetwork(int layersInf[],int num_inputs,int num_outputs,int num_layers)
{
    NeuralNetwork::bias = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/1.0));
    NeuralNetwork::num_layers = num_layers;
    NeuralNetwork::num_inputs = num_inputs;
    NeuralNetwork::num_outputs = num_outputs;
    NeuralNetwork::l = new NeuronLayer[num_layers];

    std::cout << "building layers !" << std::endl;

    // Initial layer
    NeuralNetwork::l[0] = NeuronLayer(layersInf[0],num_inputs);

    // creating middel layers
    for (int i = 1; i < num_layers+1; i++)
    {
        NeuralNetwork::l[i] = NeuronLayer(layersInf[i],layersInf[i-1]);
    }

    // creating output layer
    NeuralNetwork::l[num_layers] = NeuronLayer(num_outputs,layersInf[num_layers-1]);

    std::cout << "Finished building layers !" << std::endl << std::endl;

};

// function for printing out info about neuralnetwork object
inline void NeuralNetwork::inf()
{
  for(int i = 0; i < NeuralNetwork::num_layers+1; i++)
  {
    std::cout << "Layer " << i << std::endl << std::endl;
    NeuralNetwork::l[i].inf();
  }

}

inline float* NeuralNetwork::feedforward(float *input)
{
  NeuralNetwork::result = NeuralNetwork::l[0].feedforward(input,NeuralNetwork::bias);
  for (int i = 1; i < NeuralNetwork::num_layers; i++)
  {
    float* res = NeuralNetwork::l[i].feedforward(NeuralNetwork::result,NeuralNetwork::bias);
    NeuralNetwork::result = res;
  }
  return NeuralNetwork::l[NeuralNetwork::num_layers].feedforward(NeuralNetwork::result,NeuralNetwork::bias);
};

int main()
{
    // amount of hidden layers included in the NeuralNetwork
    int amountOfHiddenLayers = 2;
    // hidden layer description contains information on construction of connections and weight
    int hld[amountOfHiddenLayers] = { 3,  // first layer
                                      4,  // second layer
                                      2   // third layer
                                    };

    // initializing new NeuralNetwork object
    NeuralNetwork NN = NeuralNetwork(hld,2,1,amountOfHiddenLayers);

    // setting up inputs
    float inputs[2] = {2.0,1.1};

    // printing NeuralNetwork information
    //NN.inf();

    // test feedforward
    std::cout << "feedforward result : " << *NN.feedforward(inputs) << std::endl;

    return 0;
};
