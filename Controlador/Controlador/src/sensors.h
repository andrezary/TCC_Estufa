#ifndef SENSORS_H
#define SENSORS_H

namespace sensors
{
    enum tipo
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
        String name;
        tipo tipo;
        int porta;
        calibracao calib;

    private:
        /* data */
    public:
        Sensor(/* args */);
        ~Sensor();
    };
}
#endif