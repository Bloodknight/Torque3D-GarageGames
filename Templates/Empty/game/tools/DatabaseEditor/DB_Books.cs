%BookDB = "books.db";

function CreateDB_Books()
{
   %sqlite = new SQLiteObject(sqlite);
   if (%sqlite == 0)
   {
      echo("ERROR: Failed to create SQLiteObject. sqliteTest aborted.");
      return;
   }
   
   // open database
   if (sqlite.openDatabase(""@%BookDB@"") == 0)
   {
      echo("ERROR: Failed to open database: " @ %dbname);
      echo("       Ensure that the disk is not full or write protected.  sqliteTest aborted.");
      sqlite.delete();
      return;
   }
   
   // create a new simple table for demonstration purposes
   %query = "CREATE TABLE Books (BookID AUTONUMBER, Title VARCHAR(200), Contents TEXT, Special INTEGER)";
   %result = sqlite.query(%query, 0);
   if (%result == 0)
   {
      // query failed
      echo("ERROR: Failed to create new table.  sqliteTest aborted.");
      sqlite.closeDatabase();
      sqlite.delete();
      return;
   }

   sqlite.clearResult(%result);

   // close database
   sqlite.closeDatabase();
   
   // delete SQLite object.
   sqlite.delete();
}

function DB_Books_insert()
{
   %vartitle      =Book_Title.getText();
   %varcontents   =Book_Content.getText();
   %varspecial    =Book_Special.getText();
   
   //MessageBoxOK(""@%this@" - DB_Books_insert()", "INSERT INTO Books VALUES (NULL, '"@%vartitle@"', '"@%varcontents@"', "@%varspecial@")");
   
   %sqlite = new SQLiteObject(sqlite);
   if (%sqlite == 0)
   {
      echo("ERROR: Failed to create SQLiteObject. sqliteTest aborted.");
      return;
   }
   
   // open database
   if (sqlite.openDatabase("books.db") == 0)
   {
      echo("ERROR: Failed to open database: " @ %dbname);
      echo("       Ensure that the disk is not full or write protected.  sqliteTest aborted.");
      sqlite.delete();
      return;
   }
   
   %query = "INSERT INTO Books VALUES (NULL, '"@%vartitle@"', '"@%varcontents@"', "@%varspecial@")";
   %result = sqlite.query(%query, 0);
   if (%result == 0) 
   {
      echo("ERROR");
   }else{
      //MessageBoxOK(""@%this@" - DB_Books_insert()", "Book added to Database");
      Book_Title.setText("");
      Book_Content.setText("");
      Book_Special.setText("");
      //echo("Transaction complete");
   }
 
   sqlite.clearResult(%result);
   sqlite.closeDatabase();
   
   sqlite.delete();
   
   // Update the List of books after adding new one
   DB_Books_List();
      
}

function DB_Books_List(){

Book_List.clear();

%sqlite = new SQLiteObject(sqlite);
if (%sqlite == 0)
{
   echo("ERROR: Failed to create SQLiteObject. sqliteTest aborted.");
   return;
}

// open database
if (sqlite.openDatabase("books.db") == 0)
{
   echo("ERROR: Failed to open database: " @ %dbname);
   echo("       Ensure that the disk is not full or write protected.  sqliteTest aborted.");
   sqlite.delete();
   return;
}

%query = "select * from Books;";
%result = sqlite.query(%query, 0);   
if (%result == 0)
{
   echo("ERROR: blah blah");
}
else
{
   while (!sqlite.endOfResult(%result))
   {
      %BookID = sqlite.getColumn(%result, "BookID");               
      %booktitle = sqlite.getColumn(%result, "title");         
      Book_List.addRow(%BookID, %booktitle);
      //echo(%bookID@", "@%booktitle@"\n");
      sqlite.nextRow(%result);
   }

}

sqlite.clearResult(%result);
sqlite.closeDatabase();

sqlite.delete();
}

function DB_Books_Action()
{
   %buttontext = Book_Action.getText();
   //MessageBoxOK("Action Button Pressed", ""@Book_Action.getText()@"");
   if (%buttontext $= "Add Book")
   {
      DB_Books_insert();
   }
   
   if (%buttontext $= "Update Book")
   {
      DB_Books_update();
   }
      
}

function DB_Books_view()
{
   Book_Action.setVisible(true);
   %sqlite = new SQLiteObject(sqlite);
   if (%sqlite == 0)
   {
      echo("ERROR: Failed to create SQLiteObject. sqliteTest aborted.");
      return;
   }
   
   // open database
   if (sqlite.openDatabase("books.db") == 0)
   {
      echo("ERROR: Failed to open database: " @ %dbname);
      echo("       Ensure that the disk is not full or write protected.  sqliteTest aborted.");
      sqlite.delete();
      return;
   }
   
   %query = "SELECT * FROM Books WHERE BookID="@Book_List.getSelectedId()@"";
   //MessageBoxOK("SQL Query", %query);
   %result = sqlite.query(%query, 0);
   if (%result == 0) 
   {
      echo("ERROR");
   }else{

      Book_Title.setText(sqlite.getColumn(%result, "Title"));
      Book_Content.setText(sqlite.getColumn(%result, "Contents"));
      Book_Special.setText(sqlite.getColumn(%result, "Special"));
      
      Book_Action.setVisible(false);
      Book_Cancel.setVisible(true);
   }
   
   sqlite.clearResult(%result);
   sqlite.closeDatabase();
   
   sqlite.delete();
   

}

function DB_Books_cancel()
{
   Book_Title.setText("");
   Book_Content.setText("");
   Book_Special.setText("0");
   Book_Action.setVisible(true);
   Book_Action.setText("Add Book");
   Book_Cancel.setVisible(false);
}

function DB_Books_edit()
{
   Book_Action.setVisible(true);
   %sqlite = new SQLiteObject(sqlite);
   if (%sqlite == 0)
   {
      echo("ERROR: Failed to create SQLiteObject. sqliteTest aborted.");
      return;
   }
   
   // open database
   if (sqlite.openDatabase("books.db") == 0)
   {
      echo("ERROR: Failed to open database: " @ %dbname);
      echo("       Ensure that the disk is not full or write protected.  sqliteTest aborted.");
      sqlite.delete();
      return;
   }
   
   %query = "SELECT * FROM Books WHERE BookID="@Book_List.getSelectedId()@"";
   //MessageBoxOK("SQL Query", %query);
   %result = sqlite.query(%query, 0);
   if (%result == 0) 
   {
      echo("ERROR");
   }else{

      Book_Title.setText(sqlite.getColumn(%result, "Title"));
      Book_Content.setText(sqlite.getColumn(%result, "Contents"));
      Book_Special.setText(sqlite.getColumn(%result, "Special"));
      
      Book_Action.setText("Update Book");
      Book_Cancel.setVisible(true);
   }
   
   sqlite.clearResult(%result);
   sqlite.closeDatabase();
   
   sqlite.delete();
   
}

function DB_Books_update()
{
   %vartitle      =Book_Title.getText();
   %varcontents   =Book_Content.getText();
   %varspecial    =Book_Special.getText();
   
   //MessageBoxOK(""@%this@" - DB_Books_insert()", "INSERT INTO Books VALUES (NULL, '"@%vartitle@"', '"@%varcontents@"', "@%varspecial@")");
   
   %sqlite = new SQLiteObject(sqlite);
   if (%sqlite == 0)
   {
      echo("ERROR: Failed to create SQLiteObject. sqliteTest aborted.");
      return;
   }
   
   // open database
   if (sqlite.openDatabase("books.db") == 0)
   {
      echo("ERROR: Failed to open database: " @ %dbname);
      echo("       Ensure that the disk is not full or write protected.  sqliteTest aborted.");
      sqlite.delete();
      return;
   }
   
   %query = "UPDATE Books SET title='"@%vartitle@"', Contents='"@%varcontents@"', Special="@%varspecial@" WHERE BookID="@Book_List.getSelectedId()@"";
   //MessageBoxOK("SQL Query", %query);
   %result = sqlite.query(%query, 0);
   if (%result == 0) 
   {
      echo("ERROR");
   }else{
      MessageBoxOK(""@%this@" - DB_Books_insert()", "Book added to Database");
      Book_Title.setText("");
      Book_Content.setText("");
      Book_Special.setText("");
   }
 
   sqlite.clearResult(%result);
   sqlite.closeDatabase();
   
   sqlite.delete();
   
   // Update the List of books after adding new one
   DB_Books_List();
      
}










