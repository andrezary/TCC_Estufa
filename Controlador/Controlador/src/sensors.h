#ifndef SENSORS_H
#define SENSORS_H

namespace sensors
{
    enum Tipo
    {
        temperatura,
        umidade,
        luz,
        outros
    };

    class calibracao
    {
    private:
        /* data */
    public:
        virtual ~calibracao() = 0;
    };

    class RegressaoLinear : public calibracao
    {
    private:
        /* data */
    public:
        RegressaoLinear(/* args */);
        ~RegressaoLinear();
    };

    class RegressaoSimples : public calibracao
    {
    private:
        /* data */
    public:
        RegressaoSimples(/* args */);
        ~RegressaoSimples();
    };

    class RegressaoTrechos : public calibracao
    {
    private:
        /* data */
    public:
        RegressaoTrechos(/* args */);
        ~RegressaoTrechos();
    };

    class Sensor
    {
    private:
        String name;
        Tipo tipo;
        int porta;
        calibracao* calib;
        bool estadoAcionador;
        int portaAcionador;
        int valorSubida;
        int valorDescida;

        /* data */
    public:
        String getName();
        void setName(const char *str);

        Tipo getTipo();
        void setTipo(Tipo type);

        int getPorta();
        void setPorta(int value);

        int getValor();
        int getRawValue();
        void setRawValue(int value);
        void setValor(int value);
        
        bool getEstadoAcionador();
        int getPortaAcionador();
        void setPortaAcionador(int value);

        int getValorSubida();
        int getValorDescida();
        void setValorSubida(int value);
        void setValorDescida(int value);

        Sensor(/* args */);
        ~Sensor();
    };
}
#endif