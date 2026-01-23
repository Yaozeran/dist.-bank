# Distributed-Bank Specification (SPEC)

## 1. Project Context

- **Objective:** Implement a client-side interface for a Distributed Banking System using UDP-based communication.


- **Architecture:** Client-Server model where the client invokes remote services (RPC) and handles callbacks for monitoring.


- **Core Constraint:** No high-level serialization libraries (RMI, Java Serialization, etc.) allowed. All marshalling/unmarshalling must be manual.



## 2. Technical Stack & Communication

- **Protocol:** UDP Sockets.


- **Data Representation:** All messages are sent as raw byte arrays.


- **Serialization:** Utilize the team's custom C serialization library.


- **Fragmentation:** Large payloads must be manually fragmented if they exceed the 1200-byte UDP limit.


-  **Endianness:** Use Network Byte Order (Big-Endian) for all numeric values.



## 3. Message Formats

Follow these structures (Based on the project log):

### Request Format (Client to Server)

| Field | Type | Description |
| --- | --- | --- |
| **Request ID** | `int` | Unique identifier for duplicate detection.
| **Op Code** | `int` | Identifies the service (Open, Close, Deposit, etc.).
| **Content** | `byte[]` | Marshaled arguments (name, password, amount, etc.).

### Response Format (Server to Client)

| Field | Type | Description |
| --- | --- | --- |
| **Status Code** | `int` | 0 for success, non-zero for specific errors (e.g., wrong password).
| **Payload** | `byte[]` | Marshaled return data (e.g., new account number or balance).

## 4. Required Client Services

You must implement an interface  that allows users to:

1. **Open Account:** Provide name, password, currency type, and initial balance.


2. **Close Account:** Provide name, account number, and password.


3. **Deposit/Withdraw:** Provide credentials, currency type, and amount.


4. **Monitor Updates:** Register with a "monitor interval" (in seconds).


    - **Blocking Behavior:** During this interval, the client blocks user input and listens for server callbacks.


5. **Custom Operations:** One idempotent and one non-idempotent operation (to be designed by the team).



## 5. Fault Tolerance & Semantics

The client must be able to switch between two invocation semantics via command-line arguments:

- **At-Least-Once:** * Implement **Timeouts** for every request.


- **Retransmit** the request if no reply is received within the timeout.

- **At-Most-Once:** * Same as above, but rely on the server's history filtering to prevent duplicate execution of non-idempotent tasks.


- **Loss Simulation:** Include a toggle to randomly "drop" outgoing requests or incoming replies to test these semantics.



## 6. Implementation Notes for Vibe Coding

- **Manual Marshalling:** For variable-length strings (like account holder names), prefix the string with its length in the byte array.


- **User Interface:** A text-based console interface is sufficient.


- **Termination:** Include a clear menu option for the user to exit the client cleanly.
