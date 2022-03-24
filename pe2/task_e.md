### Task E - Extra credit

Because of how the webserver is implemented, one can access other files than the ones meant to. This is done by explotiong the www-path. 
Users can access other files by for example using "/" and change the filepath. 

In order to fix these problems there would have to be implemented some sort of filter or control check to account for the changed URL. 
In addition, it is not only necessary to filter out the slashes but also their corresponding ascii values, because this also can be abused. 

Furthermore, the program could be designed to show a "404 - Not Found" page if any page other than the chosen ones were accessed, however this would require a monumental amount of code and validation in order to meet the rigourous standard necessary. 