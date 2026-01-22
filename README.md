

# Overview

This project is a Minimum Viable Product (MVP) for a restaurant Point-of-Sale (POS) system, implemented as a Windows desktop application.
It models the full restaurant workflow, from menu management and table seating to orders, kitchen tickets, billing, and payments using a clean, modular domain design.

The focus of this MVP is business logic, validation, and system architecture rather than persistence or UI polish.

Note: This version does not include persistence. All data is in-memory and resets when the application closes.

# Key Features

- Menu and item availability management
- Table layout and seating logic
- Reservations and walk-ins with wait-queue handling
- Order creation and validation
- Kitchen ticket routing and station workflow
- Billing, cashier flow, and payment processing
- Clear separation of responsibilities across managers and services

# Architecture Overview

The system is designed around explicit domain entities and manager classes that coordinate workflows without tight coupling.

## Core Domains

- ### Menu & Ordering
	- MenuItem, Menu, Order, OrderManager, OrderValidation

- ### Front of House
	- Table, TableManager
	- Reservation, ResvManager
	- WalkIn, WalkInManager
	- Host, Server

- ### Kitchen Workflow
	- KitchenTicket, KticketManager
	- Station, Expeditor, RunnerStation

- ### Billing & Payments
	- Bill, Cashier, Payment

- ### System Aggregate
	- Restaurant - acts as a high-level composition root
	- main.cpp 

Each class has a single responsibility, and validation is enforced at boundaries (e.g., invalid prices, quantities, IDs).

## Design Highlights

- Strong validation using explicit checks and exceptions for invalid state
- Value semantics with careful use of move operations to avoid unnecessary copies
- Clear identity rules (e.g., SKU-based menu items, ID-based entities)
- Reference wrappers used where appropriate to avoid accidental object copying
- Manager classes isolate storage and lifecycle concerns
- Status enums make state transitions explicit and readable

## Current Limitations (MVP Scope)
- No database or file persistence (in-memory only)
- Single-process, single-user model
- No authentication or roles yet.
- Simplified payment processing (simulated success/failure)
- UI is minimal and focused on demonstrating workflow

These constraints are intentional to keep the MVP focused on correctness and clarity.

## Planned Improvements

- Persistence layer (SQLite or file-based storage)
- Transaction history and reporting
- Multi-user roles (host, server, cashier)
- Barcode / SKU scanning
- Robust error logging
- Thread-safe time formatting utilities

## How to Run
### Requirements
- Windows 7 or later or Linux-based OS
- No additional dependencies
- No internet connection required

### Steps
- Download and unzip the project folder
- Double-click POS.exe
- The application will start with sample in-memory data
- Interact with the system to create orders, seat tables, and process bills
- Edit the main file to try different commands


#### Windows may show a SmartScreen warning because this executable is unsigned. This is expected for demo software.


The application:
- Does not access the network
- Does not modify system files
- Stores all data in memory only

## Why This Project

This MVP demonstrates my ability to:
- Translate real-world business workflows into code
- Design maintainable domain models
- Enforce correctness through validation
- Build systems incrementally with clear extension points

I am happy to walk through the architecture, tradeoffs, and future roadmap in an interview.

All rights reserved.