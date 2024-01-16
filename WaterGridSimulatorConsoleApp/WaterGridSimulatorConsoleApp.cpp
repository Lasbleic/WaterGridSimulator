// WaterGridSimulatorConsoleApp.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include "WaterGridSimulatorConsoleApp.hpp"
#include "Command.hpp"


WaterGridSimulator createNewWaterGridSimulatorWizard() {
	std::cout << "Please enter the number of rows of the water grid: ";
	// Ask user for number of rows
	int numberRow;
	std::cin >> numberRow;
	std::cout << "Please enter the number of columns of the water grid: ";
	// Ask user for number of columns
	int numberColumn;
	std::cin >> numberColumn;
	// Create a new WaterGridSimulator with the given number of rows and columns
	WaterGridSimulator waterGridSimulator = createWaterGridSimulator(numberRow, numberColumn);
	std::cout << waterGridSimulator.getCellGrid() << std::endl;
	return waterGridSimulator;
}

WaterGridSimulator createWaterGridSimulator(int numberRow, int numberColumn) {
	WaterGridSimulator waterGridSimulator{ numberRow, numberColumn };
	for (int i = 0; i < numberRow; i++) {
		for (int j = 0; j < numberColumn; j++) {
			if (i == 0 || i == numberRow - 1 || j == 0 || j == numberColumn - 1) {
				waterGridSimulator.addFloor(i, j, 2);
			}
		}
	}
	if (numberRow > 2 && numberColumn > 2) {
		waterGridSimulator.addWater(1, 1, (numberRow - 2) * (numberColumn - 2));
	}
	return waterGridSimulator;
}

int main()
{
	std::cout << "Welcome to the Water Grid Simulator!\n";
	WaterGridSimulator waterGridSimulator = createNewWaterGridSimulatorWizard();
	while (true) {
		std::cout << "> ";
		Command command;
		try {
			std::cin >> command;
		}
		catch (std::invalid_argument& e) {
			std::cout << "Unknown command. Commands are :" << std::endl;
			std::cout << " -\"new\"" << std::endl;
			std::cout << " -\"exit\"" << std::endl;
			std::cout << " -\"addWater [row] [column] [volume]\"" << std::endl;
			std::cout << " -\"addFloor [row] [column] [level]\"" << std::endl;
			continue;
		}
		if (command.getCommandType() == CommandType::Exit) {
			break;
		}
		if (command.getCommandType() == CommandType::NewWaterGridSimulator) {
			waterGridSimulator = createNewWaterGridSimulatorWizard();
		}
		else 
		{
			command.execute(waterGridSimulator);
			std::cout << waterGridSimulator.getCellGrid() << std::endl;
		}
	}
}

// Exécuter le programme : Ctrl+F5 ou menu Déboguer > Exécuter sans débogage
// Déboguer le programme : F5 ou menu Déboguer > Démarrer le débogage

// Astuces pour bien démarrer : 
//   1. Utilisez la fenêtre Explorateur de solutions pour ajouter des fichiers et les gérer.
//   2. Utilisez la fenêtre Team Explorer pour vous connecter au contrôle de code source.
//   3. Utilisez la fenêtre Sortie pour voir la sortie de la génération et d'autres messages.
//   4. Utilisez la fenêtre Liste d'erreurs pour voir les erreurs.
//   5. Accédez à Projet > Ajouter un nouvel élément pour créer des fichiers de code, ou à Projet > Ajouter un élément existant pour ajouter des fichiers de code existants au projet.
//   6. Pour rouvrir ce projet plus tard, accédez à Fichier > Ouvrir > Projet et sélectionnez le fichier .sln.
