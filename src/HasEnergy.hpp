#ifndef HAS_ENERGY_HPP_INCLUDED
#define HAS_ENERGY_HPP_INCLUDED

struct HasEnergy
{
    float energy;

    HasEnergy(float energy, HasEnergy *source = 0) : energy(energy)
    {
        if (source)
            source->energy-= energy;
    }

    void takeEnergy(HasEnergy *source, float energy)
    {
        if (source && source->energy <= 0)
            return;

        if (source)
            source->energy-= energy;
        this->energy+= energy;
    }

    void giveEnergy(HasEnergy *source, float energy)
    {
        if (energy <= 0)
            return;

        if (source)
            source->energy+= energy;
        this->energy-= energy;
    }

    void takeAllEnergy(HasEnergy *source)
    {
        if (source->energy <= 0)
            return;

        this->energy+= source->energy;
        source->energy-= source->energy;
    }

    void giveAllEnergy(HasEnergy *source)
    {
        if (energy <= 0)
            return;
            
        source->energy+= this->energy;
        this->energy-= this->energy;
    }
};

#endif//HAS_ENERGY_HPP_INCLUDED
