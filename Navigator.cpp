/***
 * File: Navigator.cpp
 * Assignment: CMSC 202, Project 3
 * Author: Christian Stenoff
 * Date Created: 03/16/2022
 * Section: 54
 * E-mail: cstenof1@umbc.edu
 * Description: Contains the class that manages the loading of files,
 *              user input, and routes. Also contains vector of all ports
 *              and all routes.
***/

#include "Navigator.h"

using namespace std;


// Name: Navigator (string) - Overloaded Constructor
// Desc: Creates a navigator object to manage routes
// Preconditions:  Provided with a filename of ports to load
// Postconditions: m_filename is populated with fileName
Navigator::Navigator(string fileName)
{
    m_fileName = fileName;
}


// Name: Navigator (destructor)
// Desc: Deallocates all dynamic aspects of a Navigator
// Preconditions: There is an existing Navigator
// Postconditions: All ports and routes are cleared
Navigator::~Navigator()
{
    for (unsigned int i = 0; i < m_routes.size(); i++)
    {
        delete m_routes.at(i);
        m_routes.at(i) = nullptr;
    }

    for (unsigned int i = 0; i < m_ports.size(); i++)
    {
        delete m_ports.at(i);
        m_ports.at(i) = nullptr;
    }
    cout << "ROUTES deleted.\nPORTS deleted." << endl;
}


// Name: Start
// Desc: Loads the file and calls the main menu
// Preconditions: m_fileName is populated
// Postconditions: All ports are loaded and the main menu runs
void Navigator::Start()
{
    ReadFile();

    MainMenu();
}


// Name: DisplayPorts
// Desc: Displays each port in m_ports
// Preconditions: At least one port is in m_ports
// Postconditions: Displays all ports. Uses overloaded << provided in Port.h
void Navigator::DisplayPorts()
{
    unsigned int numPorts = m_ports.size();

    cout << "Ports:" << endl;

    for (unsigned int i = 0; i < numPorts; i++)
    {
        cout << (i+1) << ". " << m_ports.at(i)->GetName() << ", " <<
        m_ports.at(i)->GetLocation() << endl;
    }
}


// Name: ReadFile
// Desc: Reads in a file that has data about each port including name, location, degrees
//       north and degrees west. Dynamically allocates ports and stores them in m_ports
//       The vector can hold many ports.
// Preconditions: Valid file name of ports
// Postconditions: Dynamically allocates each port and enters it into m_ports
void Navigator::ReadFile()
{
    // Declare variables needed
    string portName;
    string portLocation;
    string degNorth;
    string degWest;
    double degreesNorth;
    double degreesWest;
    Port * tempPort;

    // Open file
    ifstream portListDoc;
    portListDoc.open(m_fileName);
    cout << "Opened File" << endl;

    // Get port information
    while (getline(portListDoc,portName,',') &&
            getline(portListDoc,portLocation,',') &&
            getline(portListDoc,degNorth,',') &&
            getline(portListDoc,degWest,'\n'))
    {
        // Convert coordinates from strings to doubles
        degreesNorth = stod(degNorth);
        degreesWest = stod(degWest);

        // Create port
        tempPort = new Port(portName,portLocation,degreesNorth,degreesWest);

        // Add port to m_ports vector
        m_ports.push_back(tempPort);
    }
    // Close file
    portListDoc.close();
    cout << "Ports loaded: " << m_ports.size() << endl;
}


// Name: InsertNewRoute
// Desc: Dynamically allocates a new route with the user selecting each port in the route.
//       Route named based on the first and last port automatically
//       For example, Baltimore to Boston
//       Once route is created, inserted into m_routes.
// Preconditions: Populated m_routes
// Postconditions: Inserts a new route into m_routes
void Navigator::InsertNewRoute()
{
    // Declare new route
    Route * tempRoute = new Route();
    int maxPortNum = m_ports.size();
    int portChoice = 0;

    // User input port selection until user terminates
    while (portChoice != (-1))
    {
        cout << "\nEnter the number of the port to add to your Route: "
                "(-1 to end)" << endl;
        cin >> portChoice;

        // Validate user port selection
        while ((portChoice < -1) or (portChoice == 0) or
        (portChoice > maxPortNum))
        {
            cout << "Please enter a valid choice between 1 and " <<
                 maxPortNum << ". (-1 to end)" << endl;
            cin >> portChoice;
        }
        if (portChoice != -1)
        {
            // Add port to end of route
            tempRoute->InsertEnd(m_ports.at(portChoice - 1)->GetName(),
                                 m_ports.at(portChoice - 1)->GetLocation(),
                                 m_ports.at(portChoice - 1)->GetNorth(),
                                 m_ports.at(portChoice - 1)->GetWest());
        }
    }

    // Only add route to m_routes if there are at least 2 ports in the route
    if (tempRoute->GetSize() > 1)
    {
        m_routes.push_back(tempRoute);
        cout << "A route named " << tempRoute->GetName() <<
        " has been created.\n" << endl;
    }
    else if (tempRoute->GetSize() < 2)
    {
        cout << "Route discarded.\n" << endl;
        cout << "You must add at least 2 ports to make a complete"
                " route.\n" << endl;

        // Delete discarded port
        delete tempRoute;
    }
}

// Name: MainMenu
// Desc: Displays the main menu and manages exiting
// Preconditions: Populated m_ports
// Postconditions: Exits when someone chooses 5
void Navigator::MainMenu()
{
    // Declare user main menu choice variable
    int mainChoice = 0;

    // Loop until user exits
    while (mainChoice != 5)
    {
        cout << "What would you like to do?:" << endl;
        cout << "1. Create New Route" << endl;
        cout << "2. Display Route" << endl;
        cout << "3. Remove Port From Route" << endl;
        cout << "4. Reverse Route" << endl;
        cout << "5. Exit" << endl;
        cin >> mainChoice;

        // Validate user main menu choice
        while ((mainChoice < 1) or (mainChoice > 5))
        {
            cout << "Please enter your choice as an integer from 1 to 5.\n"
            << endl;
            cout << "What would you like to do?:" << endl;
            cout << "1. Create New Route" << endl;
            cout << "2. Display Route" << endl;
            cout << "3. Remove Port From Route" << endl;
            cout << "4. Reverse Route" << endl;
            cout << "5. Exit" << endl;
            cin >> mainChoice;
        }

        // Function calls based on user main menu choice
        if (mainChoice == 1)
        {
            DisplayPorts();
            InsertNewRoute();
        }
        else if (mainChoice == 2)
        {
            DisplayRoute();
        }
        else if (mainChoice == 3)
        {
            RemovePortFromRoute();
        }
        else if (mainChoice == 4)
        {
            ReverseRoute();
        }
        else if (mainChoice == 5)
        {
            cout << "Thanks for playing!" << endl;
        }
    }
}


// Name: ChooseRoute
// Desc: Allows user to choose a specific route to work with by displaying numbered list
// Preconditions: Populated m_routes
// Postconditions: Returns the index of the selected route minus 1
int Navigator::ChooseRoute()
{
    // Declare user route choice variable
    int routeChoice = 0;

    unsigned int routesSize = m_routes.size();

    // If no routes available:
    if (routesSize == 0)
    {
        cout << "There are no routes available. Please create a route.\n"
        << endl;
    }

    // Else, Route exists
    else
    {
        cout << "\nWhich route would you like to use?" << endl;

        // Display available routes for user to choose
        for (int i = 0; i < int (m_routes.size()); i++)
        {
            cout << (i + 1) << ". " << m_routes.at(i)->GetName() << endl;
        }

        // User route choice
        cin >> routeChoice;

        // Validate user route choice
        while (routeChoice < 1 or routeChoice > int (m_routes.size()))
        {
            cout << "\nPlease enter a valid route between 1 and "
            << m_routes.size() << endl;
            cin >> routeChoice;
        }
    }
    // Account for difference between vector index and display index
    return (routeChoice - 1);
}


// Name: DisplayRoute
// Desc: Using ChooseRoute, displays a numbered list of all routes.
//       If no routes, indicates that there are no routes to display
//       User selects a route from list to display
//       Displays numbered list of each port in route
//       Displays total miles of a route using RouteDistance
// Preconditions: Routes has more than one port
// Postconditions: Displays all ports in a route and the total miles of the route
void Navigator::DisplayRoute()
{
    // If no routes exist:
    if (int (m_routes.size()) == 0)
    {
        cout << "There are no routes to display.\n" << endl;
    }
    // Else, routes exist:
    else
    {
        //Get user route choice via ChooseRoute function
        int routeChoice = ChooseRoute();
        cout << endl;

        // Display chosen route via DisplayRoute function from Routes.h
        m_routes.at(routeChoice)->DisplayRoute();

        // Display distance of route using RouteDistance function
        cout << "\nThe total distance of this route is " <<
        RouteDistance(m_routes.at(routeChoice)) <<
        " miles.\n" << endl;
    }
}


// Name: RemovePortFromRoute()
// Desc: Using ChooseRoute, displays a numbered list of all routes.
//       User selects one of the routes to remove a port from.
//       Displays a numbered list of all ports in selected route.
//       User selects port to remove from list.
//       Removes port from route. If first or last port removed, updates name of route.
// Preconditions: Routes has more than one port
// Postconditions: Displays updated route with removed port and new name
void Navigator::RemovePortFromRoute()
{
    // If no routes exist:
    if (int (m_routes.size()) == 0)
    {
        cout << "There are no routes to edit.\n" << endl;
    }
    // Else, routes exist:
    else
    {
        // Get user route choice via ChooseRoute function
        int routeChoice = ChooseRoute();

        // Declare user port choice variable and display available ports in the
        // route
        int portChoice;
        m_routes.at(routeChoice)->DisplayRoute();
        cout << "\nWhich port would you like to remove? (-1 to cancel)" << endl;
        cin >> portChoice;

        while (portChoice != -1)
        {
            // Validate user port choice
            while ((portChoice < -1) or (portChoice == 0) or
                   (portChoice > int(m_routes.at(routeChoice)->GetSize())))
            {
                while (portChoice != -1)
                {
                    cout << "\nPlease enter a valid port in the route, from 1 to " <<
                         m_routes.at(routeChoice)->GetSize() << endl;
                    cin >> portChoice;
                }
            }

            // Remove chosen port from chosen route
            m_routes.at(routeChoice)->RemovePort(portChoice - 1);

            // Set portChoice to -1 to prevent a recurring removal loop
            portChoice = -1;

            // Update name of chosen route
            m_routes.at(routeChoice)->
                    SetName(m_routes.at(routeChoice)->UpdateName());

            cout << "\nRoute " << routeChoice + 1 <<
            " has been updated and is now:" << endl;

            // Display updated route message
            m_routes.at(routeChoice)->DisplayRoute();
            cout << endl;
        }
    }
}



// Name: RouteDistance
// Desc: Calculates the total distance of a route
//       Goes from port 1 to port 2 then port 2 to port 3 and repeats for
//       length of route. Calculates the distance using CalcDistance (provided)
//       Aggregates the total and returns the total in miles
// Preconditions: Populated route with more than one port
// Postconditions: Returns the total miles between all ports in a route
double Navigator::RouteDistance(Route * chosenRoute)
{
    // Declare variables needed for summation
    double totalDistance = 0;
    double intervalDistance;

    // Declare index for starting calculation that loops through the route
    int mHeadIndex = 0;
    int mHeadNextIndex = 1;
    // Declare route size used to stop looping
    int routeSize = chosenRoute->GetSize();

    // Declare pointers
    Port * curr = chosenRoute->GetData(mHeadIndex);
    Port * next = chosenRoute->GetData(mHeadNextIndex);
    Port * last = chosenRoute->GetData(routeSize - 1);

    // Loop through route until the end, adding each interval distance
    // to the total distance of the route
    while (curr != last)
    {
        intervalDistance = CalcDistance( //DO I NEED THE NAVIGATOR:: HERE?
                curr->GetNorth(),curr->GetWest(),
                next->GetNorth(),next->GetWest());

        totalDistance = totalDistance + intervalDistance;

        curr = curr->GetNext();
        next = curr->GetNext();
    }
    return totalDistance;
}


// Name: ReverseRoute
// Desc: Using ChooseRoute, users chooses route  and the route is reversed
//       If no routes in m_routes, indicates no routes available to reverse
//       Renames route based on new starting port and ending port
//       For example, Baltimore to Boston becomes Boston to Baltimore
// Preconditions: Populated m_routes
// Postconditions: Reverses a specific route by reversing the ports in place
//                 Must move ports, cannot just change data in ports.
void Navigator::ReverseRoute()
{
    // If no routes exist:
    if (int (m_routes.size())== 0)
    {
        cout << "There are no routes to reverse.\n" << endl;
    }
    // Else, route exists
    else
    {
        // Get user route choice
        int routeChoice = ChooseRoute();

        // Reverse chosen route
        m_routes.at(routeChoice)->ReverseRoute();

        // Display message
        cout << "\nThe " << m_routes.at(routeChoice)->GetName() <<
        " route has been reversed." << endl;

        // Update name of reversed route
        m_routes.at(routeChoice)->
        SetName(m_routes.at(routeChoice)->UpdateName());

        // Display updated route
        cout << "\nThe updated route is now:" << endl;

        m_routes.at(routeChoice)->DisplayRoute();
        cout << endl;
    }
}
