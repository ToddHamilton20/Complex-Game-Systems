// Author:					Todd Hamilton
// Last Change:				28/04/16
// Description of Change:	Commented
// Description of File:		Genetics base class, all genetic applications should derive from this.

#pragma once

#include <vector>
#include <thread>
#include <mutex>
#include <random>
#include <atomic>

template <typename T>
struct Chromosome
{
	Chromosome(int a_geneLength) : fitness(0)
	{
		genes.resize(a_geneLength);
	}

	std::vector<T> genes;
	float fitness;
};

template <typename T>
class Genetics
{
public:
	Genetics(int a_geneLength) : geneLength(a_geneLength)
	{
		initialised = false;
		threadsRunning = false;
	}

	// Reserves population and child memory, populates the population, resets generation count and checks intial fitness.
	// Notes:		1. If one or more threaded generation simulations are running, will stop execution and initialise after they finish.
	void Initialise(unsigned int a_populationSize, unsigned int a_childCount)
	{
		PInitialise(a_populationSize, a_childCount);
	}

	// Starts a thread which reserves population and child memory, populates the population, resets generation count and checks intial fitness.
	// Notes:		1. If one or more threaded generation simulations are running, will initialise after they finish.
	void ThreadedInitialise(unsigned int a_populationSize, unsigned int a_childCount)
	{
		std::thread(&Genetics::PInitialise, this, a_populationSize, a_childCount).detach();
	}

	// Simulates a single generation.
	// Notes:		1. If one or more threaded generation simulations are running, will stop execution and process after they finish.
	void SimulateGeneration()
	{
		PSimulateGeneration();
	}

	// Simulates a specific amount of generations.
	// Notes:		1. If one or more threaded generation simulations are running, will stop execution and process after they finish.
	void SimulateGenerations(int a_generations)
	{
		PSimulateGenerations(a_generations);
	}

	// Starts a generation simulation on a new thread.
	// Notes:		1. If one or more threaded generation simulations are running, will process after they finish.
	void SimulateThreadedGeneration()
	{
		std::thread(&Genetics::PSimulateGeneration, this).detach();
	}

	// Starts a specific amount of generation simulations on a new thread.
	// Notes:		1. If one or more threaded generation simulations are running, will process after they finish.
	void SimulateThreadedGenerations(int a_generations)
	{
		std::thread(&Genetics::PSimulateGenerations, this, a_generations).detach();
	}

	// Returns true if Initialisation has finished.
	bool Initialised()
	{
		return initialised;
	}

	// Returns true if a thread is currently running.
	bool AnyThreadsRunning()
	{
		return threadsRunning > 0;
	}

	// Returns the count of threads currently running / queued.
	unsigned int ThreadsRunning()
	{
		return threadsRunning;
	}

	// Returns the current generation depth.
	unsigned int Generation()
	{
		return generation;
	}

	// Returns the population size variable.
	inline unsigned int PopulationSize()
	{
		return populationSize;
	}

	// Returns the child count variable.
	inline unsigned int ChildCount()
	{
		return childCount;
	}

	// Returns the gene length variable.
	inline const unsigned int GeneLength()
	{
		return geneLength;
	}

protected:
	// A pool of all chromosomes:
	// Indices 0 to populationSize - 1 = population (Inclusive).
	// Indices populationSize to populationSize + childCount - 1 = children (Inclusive)
	std::vector<Chromosome<T>> populationPool;

	// Should store pointers to the reproducing chromosomes
	std::vector<Chromosome<T>*> parents;

	// Stores a pointer to the first population member (index 0 of populationPool)
	Chromosome<T>* population;

	// Stores a pointer to the first child member (index populationSize of populationPool)
	Chromosome<T>* children;

	// Function should populate the population with genes.
	virtual void Populate(std::mt19937& a_random) = 0;

	// Function should calculate fitness of each population chromosome, fitness values should be stored in the chromosome.
	virtual void Fitness(std::mt19937& a_random) = 0;

	// Function should select chromosomes to reproduce. Stores a pointer to each parent in parent pointer vector.
	virtual void Select(std::mt19937& a_random) = 0;

	// Function should perform a crossover using parent pointer vector to create children genes, children should be stored in the children array.
	virtual void Crossover(std::mt19937& a_random) = 0;

	// Function should mutate each child in children array.
	virtual void Mutate(std::mt19937& a_random) = 0;

	// Function should select a new generation from the population and children arrays. New generation should be stored in population.
	virtual void Discard(std::mt19937& a_random) = 0;

private:
	// Has Initalisation finished.
	std::atomic<bool> initialised;
	// Stores the current generation depth
	std::atomic<unsigned int> generation;
	// Stores how many threads are currently running / queued.
	std::atomic<unsigned int> threadsRunning;
	// How many of the general population.
	unsigned int populationSize;
	// How many children are born each generation.
	unsigned int childCount;
	// How many genes in a chromosome.
	const unsigned int geneLength;

	// Lock covers populationPool, population, parents and children variables.
	std::mutex populationLock;

	// Thread safe initialise function.
	void PInitialise(unsigned int a_populationSize, unsigned int a_childCount)
	{
		std::mt19937 random; random.seed(time(nullptr));

		threadsRunning++;
		initialised = false;

		{
			std::lock_guard<std::mutex> guard(populationLock);

			populationSize = a_populationSize;
			childCount = a_childCount;

			populationPool.clear();
			populationPool.reserve(a_populationSize + a_childCount);

			for (int i = 0; i < populationSize + childCount; i++)
			{
				populationPool.push_back(Chromosome<T>(geneLength));
			}

			population = &populationPool[0];
			children = &populationPool[populationSize];
			parents.resize(a_childCount * 2);

			Populate(random);
			Fitness(random);
		}

		generation = 0;
		initialised = true;
		threadsRunning--;
	}

	// Thead safe single generation function.
	void PSimulateGeneration()
	{
		std::mt19937 random; random.seed(time(nullptr));

		threadsRunning++;

		{
			std::lock_guard<std::mutex> guard(populationLock);

			Select(random);
			Crossover(random);
			Mutate(random);
			Discard(random);
			Fitness(random);
		}

		generation++;
		threadsRunning--;
	}

	// Thread safe multiple generation function.
	void PSimulateGenerations(int a_generations)
	{
		std::mt19937 random; random.seed(time(nullptr));

		threadsRunning++;

		{
			std::lock_guard<std::mutex> guard(populationLock);

			for (int i = 0; i < a_generations; i++)
			{
				Select(random);
				Crossover(random);
				Mutate(random);
				Discard(random);
				Fitness(random);

				generation++;
			}
		}

		threadsRunning--;
	}
};