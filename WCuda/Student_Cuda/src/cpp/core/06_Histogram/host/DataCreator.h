#pragma once


class DataCreator
    {

    public:
	DataCreator(int dataMax);
	virtual ~DataCreator(void);

    public:
	/**
	 * in [0,255]
	 */
	int* getTabData();
	void print();
	int getLength();


    private:
	void createDataTriangulaire();
	void melangerData();
	void swap(int i, int j);


    private:
// Inputs
	int dataMax;
// Tools
	int n;
	int nbMelange;
// Outputs
	int* tabData;
    };
