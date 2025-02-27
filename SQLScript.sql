/*DDL*/ 
CREATE TABLE "Patron" (
    userID VARCHAR(20) PRIMARY KEY,
    name VARCHAR(50) NOT NULL,
	password VARCHAR(20) NOT NULL,
    ic VARCHAR(14) NOT NULL UNIQUE,
    phoneNum VARCHAR(12) NOT NULL,
    email VARCHAR(100) NOT NULL,
    address VARCHAR(255) NOT NULL,
    role VARCHAR(20) DEFAULT "Patron" CHECK (role IN ("Patron", "Staff"))  
);

CREATE TABLE Book (
    bookID VARCHAR(20) PRIMARY KEY,
    title VARCHAR(255) NOT NULL,
    isbn VARCHAR(13) NOT NULL UNIQUE,
    author VARCHAR(100) NOT NULL,
    publisher VARCHAR(100),
    category VARCHAR(20) NOT NULL,
    publicationYear INT NOT NULL,
    quantity INT,
    status VARCHAR(10) DEFAULT "Available" CHECK (status IN ("Available", Unavailable)),
    price DECIMAL(6, 2)
);
