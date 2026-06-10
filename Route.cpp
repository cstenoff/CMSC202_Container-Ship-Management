/***
 * File: Route.cpp
 * Assignment: CMSC 202, Project 3
 * Author: Christian Stenoff
 * Date Created: 03/16/2022
 * Section: 54
 * E-mail: cstenof1@umbc.edu
 * Description: Contains functions that create and modify the routes for use
 *              in Navigator.cpp.
***/

#include "Route.h"

using namespace std;

// Name: Route() - Default Constructor
// Desc: Used to build a new Route (linked list) make up of ports
// Preconditions: None
// Postconditions: Creates a new Route where m_head and m_tail point to nullptr and size = 0
Route::Route()
{
    m_head = nullptr;
    m_tail = nullptr;
    m_size = 0;
}


// Name: SetName(string)
// Desc: Sets the name of the route (usually first port to last port)
// Preconditions: None
// Postconditions: Sets name of route
void Route::SetName(string routeName)
{
    // Get current name of route using Update name function
    routeName = UpdateName();

    // Assign updated name to m_name
    m_name = routeName;
}


// Name: ~Route() - Destructor
// Desc: Used to destruct a strand of Route
// Preconditions: There is an existing Route strand with at least one port
// Postconditions: Route is deallocated (including all dynamically allocated ports)
//                 to have no memory leaks!
Route::~Route()
{
    Port * curr = m_head;

    while (curr != nullptr)
    {
        m_head = curr;
        curr = curr->GetNext();
        delete m_head;
    }
    m_head = nullptr;
    m_tail = nullptr;
    m_size = 0;
}


// Name: InsertEnd (string, string, double double)
// Desc: Creates a new port using the data passed to it.
//       Inserts the new port at the end of the route
// Preconditions: Takes in a Port
//                Requires a Route
// Postconditions: Adds the new port to the end of a route
void Route::InsertEnd(string portName, string portLocation,
                      double degreesNorth, double degreesWest){

    // Create new port for use in new route
    Port * tempPort = new Port(portName, portLocation, degreesNorth, degreesWest);

    // If route is empty:
    if (int (m_size) == 0)
    {
        m_head = tempPort;
        m_tail = tempPort;
        m_head->SetNext(nullptr);
    }
    // Else if route has one port
    else if (int(m_size) == 1)
    {
        m_head->SetNext(tempPort);
        m_tail = tempPort;
        m_tail->SetNext(nullptr);
        SetName(UpdateName());
    }
    // Else, route has more than one port
    else
    {
        m_tail->SetNext(tempPort);
        m_tail = tempPort;
        m_tail->SetNext(nullptr);
        m_name = UpdateName();
    }
    // Increment size of route
    m_size++;
}

// Name: RemovePort(int index)
// Desc: Removes a port from the route at the index provided
//       Hint: Special cases (first port, last port, middle port)
// Preconditions: Index must be less than the size of the route
//       Cannot make route less than two ports. If the route has
//       two or fewer ports, fails.
// Postconditions: Name may be updated. Size is reduced. Route has one less port.
void Route::RemovePort(int portChoice)
{
    // Declare pointers
    Port * curr = m_head;
    Port * prev = m_head;

    // Declare counter
    int counter = 0;

    // Loop through route until user chosen port is reached
    while (curr != nullptr)
    {
        if (counter == portChoice)
        {
            prev->SetNext(curr->GetNext());

            // If chosen port is the end of the route, reassign m_tail
            if (portChoice == int (m_size - 1))
            {
                m_tail = prev;
            }
            // If chosen port is the beginning of the route, reassign m_head
            else if (portChoice == 0)
            {
                m_head = prev->GetNext();
            }

            // Remove chosen port
            delete curr;

            // Decrement route size
            m_size--;
            return;
        }
        // Iterate through route and increment counter until target port
        // is reached
        prev = curr;
        curr = curr->GetNext();
        counter++;
    }
}

// Name: GetName()
// Desc: Returns the name of the route (Usually starting port to last port)
//       For example: Baltimore to Brunswick
// Preconditions: Requires a Route
// Postconditions: Returns m_name;
string Route::GetName()
{
    return m_name;
}


// Name: UpdateName()
// Desc: Updates m_name based on the name of the first port in the route
//       and the last port in the route
//       For example: Baltimore to Brunswick
// Preconditions: Requires a Route with at least two ports
// Postconditions: Returns m_name;
string Route::UpdateName()
{
    string originName = m_head->GetName();
    string destinationName = m_tail->GetName();

    string updatedName = originName + " to " + destinationName;

    // Assign updated name to m_name
    m_name = updatedName;

    return m_name;
}


// Name: GetSize()
// Desc: Returns the number of ports in a route
// Preconditions: Requires a Route
// Postconditions: Returns m_size;
int Route::GetSize()
{
    return m_size;
}


// Name: ReverseRoute
// Desc: Reverses a route
// Preconditions: Reverses the Route
// Postconditions: Route is reversed in place; nothing returned
void Route::ReverseRoute()
{
    // If route has less than two ports
    if (int(m_size) < 2)
    {
        cout << "There are not enough routes to reverse.\n" << endl;
    }
    // Else, route has at least two ports
    else
    {
        // Declare pointers
        Port * curr = m_head;
        Port * prev = nullptr;
        Port * next;
        Port * newTail = m_head;

        // Loop through route
        while (curr != nullptr)
        {
            // Adjust port positions in route
            next = curr->GetNext();
            curr->SetNext(prev);

            prev = curr;
            curr = next;
        }
        // Assign correct ports to m_head and m_tail
        m_head = prev;
        m_tail = newTail;
    }
}


// Name: GetData (int)
// Desc: Returns a port at a specific index
// Preconditions: Requires a Route
// Postconditions: Returns the port from specific item
Port * Route::GetData(int chosenIndex)
{
    // Declare pointers
    Port * curr = m_head;
    Port * target = m_head;

    // Loop through route until chosen port is reached
    for (int i = 0; i < m_size; i++)
    {
        // If chosen port is reached: return port
        if (i == chosenIndex)
        {
            target = curr;
            return target;
        }
        // Else, continue to next port in the route
        else
        {
            curr = curr->GetNext();
        }
    }
    return target;
}


// Name: DisplayRoute
// Desc: Displays all of the ports in a route
// Preconditions: Requires a Route
// Postconditions: Displays all of the ports in a route
// Formatted: Baltimore, Maryland (N39.209 W76.517)
void Route::DisplayRoute()
{
    // If no ports exist in the route:
    if (int(m_size) == 0)
    {
        cout << "That route is empty." << endl;
    }
    // Else, ports exist in the route:
    else
    {
        // Declare pointer
        Port * curr = m_head;

        // Display route name
        cout << m_name << endl;

        // Loop through route to display each port in proper format
        for (int i = 0; i < int(m_size); i++)
        {
            cout << (i + 1) << ". " << curr->GetName() << ", " <<
                 curr->GetLocation() << " (N" << curr->GetNorth() <<
                 " W" << curr->GetWest() << ")" << endl;

            // Increment pointer for loop
            curr = curr->GetNext();
        }
    }
}
