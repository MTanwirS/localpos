MenuItem
const std::string itemsku_
std::string itemname_
double itemprice_
std::optional<std::string> itemdescription_
std::vector<std::string> itemtags_
bool itemavailable_{true}

MenuItem(const std::string& sku, const std::string& name, double price) / (std::string&& sku, std::string&& name, double price) // validate name and price and throw std::invalid_argument on invalid input.
const std::string& getSku() const
const std::string& getName() const
double getPrice() const
const std::optional<std::string>& getDescription() const
const std::vector<std::string>& getTags() const
bool isAvailable() const
void rename(const std::string& newName) / (std::string&& newName) // validates then assigns; rvalue overload uses std::move.
bool setPrice(double newPrice) // validates and returns false on invalid price.
void setDescription(const std::optional<std::string>& desc) / (std::optional<std::string>&& desc)
void setTags(std::vector<std::string>&& tags) / (const std::vector<std::string>& tags) // accept lvalue and rvalue overloads; rvalue overloads should std::move into members to avoid copies.
void setAvailable(bool available)
bool sameIdentityAs(const MenuItem& other) const // compares itemsku_ only.
private : bool isValidPrice(double p); // p >= 0.0
bool isValidName(const std::string& n) // !n.empty()
---

Order
std::string orderId_
std::string tableId_
std::map<std::string, int> orderitems_

Order(const std::string& orderId, const std::string& tableId) // validates non-empty orderId_ and tableId_ and throws on invalid.
const std::string& getOrderId() const
const std::string& getTableId() const
std::optional<int> getQuantity(const std::string& sku) const // returns std::nullopt if SKU not present.
bool addItem(const std::string& sku, int quantity) // validates quantity; if SKU exists increments quantity, else inserts.
bool removeItem(const std::string& sku)
bool updateQuantity(const std::string& sku, int newQuantity) // validates and replaces quantity only if SKU exists.
int totalItems() const // sums quantities.
const std::map<std::string, int>& items() const
bool isEmpty() const // returns orderitems_.empty().
private : bool isValidQuantity(int q) // (quantity > 0)
---

Table
uint32_t tid_
std::string tnumber_
uint32_t tseats_
TableStatus tstatus_ (Free, Occupied)
std::optional<uint64_t> torderId_

Table(uint32_t id, std::string number, uint32_t seats)
uint32_t id() const
const std::string& number() const
uint32_t seats() const
TableStatus status() const
std::optional<uint64_t> orderId() const
bool isFree() const
bool isOccupied() const
bool hasOrder() const
bool assignOrder(uint64_t orderId) // currently requires isOccupied() and no existing order; used to attach an order after seating.
void clearOrder()
bool canSeat(uint32_t partysize) const //  returns isFree() && partysize <= tseats_.
bool seatGuests(uint64_t orderId) // if free, set tstatus_ = Occupied and assign order; returns success.
bool freeTable() // sets status to Free and clears torderId_.
std::string summary() const
---

Reservation
uint64_t rid_
std::string rguestName_
uint32_t rtableId_
uint32_t rpartySize_
std::chrono::system_clock::time_point rtime_
ResvStatus rstatus_ (Booked, Cancelled, Seated, NoShow)
std::optional<std::string> rnotes_

Reservation(uint64_t id, std::string guestname, uint32_t partysize, std::chrono::system_clock::time_point time, uint32_t tableid = 0) // moves guestname and sets rstatus_ = Booked.
uint64_t id() const
const std::string& guestName() const
uint32_t tableId() const
uint32_t partySize() const
std::chrono::system_clock::time_point time() const
ResvStatus status() const
std::optional<std::string> notes() const
void setTable(uint32_t tid)
void setNotes(std::string note)
void clearNotes()
void cancel() // update rstatus_.
void complete() (sets Seated) — consider renaming to markSeated() for clarity
void markNoShow()
std::string summary() const // prints human readable reservation details.
private: std::string formatTime(const std::chrono::system_clock::time_point& tp) const (uses std::localtime — not thread safe)
---

WalkIn
uint64_t wid_
std::string wguestName_
uint32_t wpartySize_
std::chrono::system_clock::time_point warrivalTime_
std::optional<uint32_t> wtableId_
WalkInStatus wstatus_ (Waiting, Seated, Left)
std::optional<std::string> wnotes_
bool wpriority_

WalkIn(uint64_t id, std::string guestName, uint32_t partySize, std::chrono::system_clock::time_point arrivalTime, bool priority) // moves guestName, sets wstatus_ = Waiting.
uint64_t id() const
const std::string& guestName() const
uint32_t partySize() const
std::chrono::system_clock::time_point arrivalTime() const
std::optional<uint32_t> tableId() const
WalkInStatus status() const
std::optional<std::string> notes() const
bool priority() const (raw flag)
bool isPriority() const (semantic accessor)
void assignTable(uint32_t tableId) // sets wtableId_.
void setNotes(std::string note)
void clearNotes()
void markSeated() // update wstatus_.
void markLeft()
std::string summary() const
Private : formatTime (same thread-safety note)
---

Menu
std::map<std::string, MenuItem> menuitems_

bool addItem(const MenuItem& item) / (MenuItem&& item) // uses emplace with item.getSku() as key; rvalue overload uses std::move.
bool removeItem(const std::string& sku)
std::optional<MenuItem> findItem(const std::string& sku) const  // currently returns a copy of MenuItem;
bool contains(const std::string& sku) const
size_t size() const
const std::map<std::string, MenuItem>& items() const
bool isItemAvailable(const std::string& sku) const // looks up SKU and calls isAvailable() on the stored MenuItem.
---

OrderManager
std::map<std::string, Order> orders_

bool addOrder(const Order& order) / (Order&& order) // emplaces by order.getOrderId(); rvalue overload moves.
bool removeOrder(const std::string& orderId)
std::optional<Order> findOrder(const std::string& orderId) const // currently returns copy
bool contains(const std::string& orderId) const
size_t size() const
const std::map<std::string, Order>& orders() const
---

OrderValidation

std::vector<std::string> validateOrder(const Order& order, const Menu& menu)
// Iterates order.items() and for each SKU: add error if !menu.contains(sku), !menu.isItemAvailable(sku), qty <= 0
---

TableManager
std::unordered_map<uint32_t, Table> tables_

bool createTable(uint32_t id, std::string number, uint32_t seats) // checks duplicate id and emplaces Table{id, std::move(number), seats}. Validate seats > 0
bool deleteTable(uint32_t id) (consider rename to removeTable) // only removes if table exists and isFree() to avoid removing occupied tables.
std::optional<std::reference_wrapper<const Table>> findTable(uint32_t id) const
std::optional<std::reference_wrapper<const Table>> findTableByNumber(const std::string& number) const
std::optional<std::reference_wrapper<const Table>> findFreeTable(uint32_t minSeats) const // iterates tables and returns first free table with seats() >= minSeats.
size_t tableCount() const
std::vector<std::string> summaries() const
const std::unordered_map<uint32_t, Table>& tables() const
---

ResvManager
std::unordered_map<uint64_t, Reservation> reservations_
TableManager& tablemgr_

ResvManager(TableManager& tablemgr)
bool createReservation(uint64_t id, std::string guestName, uint32_t partySize, std::chrono::system_clock::time_point time, uint32_t tableid = 0) // checks duplicate id, constructs Reservation
bool cancelReservation(uint64_t id) // sets reservation status to Cancelled (keeps record).
bool assignTable(uint64_t id, uint32_t tid) // sets setTable on reservation; does not change status (Host coordinates seating).
std::optional<std::reference_wrapper<const Reservation>> findReservation(uint64_t id) const / (uint64_t id) // returns reference wrappers to avoid copies.
size_t reservationCount() const
std::vector<std::string> summaries() const
---

WalkInManager
std::unordered_map<uint64_t, WalkIn> walkins_

bool addWalkIn(uint64_t id, std::string guestName, uint32_t partySize, std::chrono::system_clock::time_point arrivalTime, bool priority = false) // emplaces a WalkIn if id not present;
bool removeWalkIn(uint64_t id) // erases entry;
std::optional<std::reference_wrapper<const WalkIn>> findWalkIn(uint64_t id) const / (uint64_t id)
size_t count() const
std::vector<std::string> summaries() const // iterates walkins_ and collects summary() strings.
---

Host
TableManager& tableMgr_
ResvManager& resvMgr_
WalkInManager& walkinMgr_
std::deque<uint64_t> waitQueue_ // FIFO

Host(TableManager& tm, ResvManager& rm, WalkInManager& wm)
bool seatReservation(uint64_t reservationId) // looks up reservation, finds table, calls resvMgr_.assignTable and optionally logs; returns success/failure.
bool seatWalkIn(uint64_t walkinId) // looks up walk-in, finds table, calls walkinMgr_.assignTable, walkin.markSeated(), and logs; returns success/failure.
void enqueueWalkin(uint64_t walkinId) // pushes back
std::optional<uint64_t> nextinQueue() //  pops front and returns id.
std::optional<uint64_t> peekQueue() const
size_t queueSize() const
std::vector<std::string> wlsummary() const // iterates waitQueue_, looks up each walk-in via walkinMgr_ and builds summary strings.
bool hasFreeTable(uint32_t minseats) const
Private : std::optional<uint32_t> findSuitableTable(uint32_t partySize) const // delegates to tableMgr_.findFreeTable(partySize) and returns the table id if present.
---

Server
uint64_t sid_
std::string sname_
OrderManager& orderMgr_
Menu& menu_
TableManager& tableMgr_

Server(uint64_t id, std::string name, OrderManager& om, Menu& m, TableManager& tm)
std::optional<std::string> createOrder(uint32_t tableId) // Checks table exists and is free, generates order ID, constructs Order, adds to OrderManager, marks table occupied.
bool addItemToOrder(const std::string& orderId, const std::string& sku, int qty) // Validates SKU exists in menu, item is available, and qty > 0; then adds to order.
bool removeItemFromOrder(const std::string& orderId, const std::string& sku) // removes item if present.
bool submitOrder(const std::string& orderId) // logs submission
std::optional<std::reference_wrapper<const Order>> getOrder(const std::string& orderId) const
std::vector<std::string> listOrders() const // summaries
uint64_t id() const / const std::string& name() const
Private : std::string generateOrderId(uint32_t tableId) const // builds simple order ID string using server id, table id, and timestamp.
bool validateItem(const std::string& sku, int qty) const // ensures quantity > 0, SKU exists, and item is available.
---

KitchenTicket
uint64_t ktid_
std::string orderId_
std::vector<std::pair<std::string,int>> items_
TicketStatus status_ (Pending, InProgress, Completed)
std::chrono::system_clock::time_point createdTime_

KitchenTicket(uint64_t id, std::string orderId, std::vector<std::pair<std::string,int>> items) // status_ = Pending and createdTime_ = now.
uint64_t id() const
const std::string& orderId() const
const std::vector<std::pair<std::string,int>>& items() const
TicketStatus status() const
std::chrono::system_clock::time_point createdTime() const
void markInProgress()
void markCompleted()
std::string summary() const // prints ticket details, items, and status.
Private helper: formatTime // human‑readable timestamps
---

KticketManager
std::unordered_map<uint64_t, KitchenTicket> tickets_

bool addTicket(const KitchenTicket& ticket)/(KitchenTicket&& ticket) // emplaces ticket if ID not present.
bool removeTicket(uint64_t ticketId)
std::optional<std::reference_wrapper<const KitchenTicket>> findTicket(uint64_t ticketId)
size_t count() const
std::vector<std::string> summaries() const
const std::unordered_map<uint64_t, KitchenTicket>& tickets() const
---

Station
uint64_t stationId_
std::string name_
std::vector<uint64_t> activeTickets_

Station(uint64_t id, std::string name)
uint64_t id() const
const std::string& name() const
const std::vector<uint64_t>& activeTickets() const
void acceptTicket(uint64_t ticketId) // pushes ticket ID into activeTickets_.
bool markTicketComplete(uint64_t ticketId) // finds and erases ticket ID from list.
std::string summary() const
---

Expeditor
KticketManager& ktMgr_
std::unordered_map<uint64_t, Station*> stations_

Methods
Expeditor(KticketManager& ktmgr)
void registerStation(Station& station)
bool routeTicket(uint64_t ticketId, uint64_t stationId) // immediate acceptance: once routed, the ticket is added to the station’s active list.
std::vector<std::string> aggregateForDispatch(uint64_t ticketId) const // just SKU + qty strings;
std::vector<std::string> kitchenSummary() const
std::optional<std::reference_wrapper<Station>> findStation(uint64_t stationId)
---

RunnerStation
uint64_t runnerId_
std::string name_
std::vector<uint64_t> deliveredTickets_

RunnerStation(uint64_t id, std::string name)
void dispatch(uint64_t ticketId)// mark ticket as delivered
std::string summary() const
---

Bill

uint64_t billId_
std::string orderId_
std::vector<std::pair<std::string,double>> items_
double subtotal_, double tax_, double total_

Bill(uint64_t id, std::string orderId)
void addItem(const std::string& sku, double price)
void calculateTotals(double taxRate)
std::string summary() const
---

Cashier
std::unordered_map<uint64_t, Bill> bills_

bool addBill(const Bill& bill) / bool addBill(Bill&& bill)
bool removeBill(uint64_t billId)
std::optional<std::reference_wrapper<Bill>> findBill(uint64_t billId)
bool settleBill(uint64_t billId) // marks bill as paid (MVP: erase bill)
size_t count() const
std::vector<std::string> summaries() const
---

Payment
uint64_t paymentId_
uint64_t billId_
std::string method_
double amount_
bool success_

Payment(uint64_t id, uint64_t billId, std::string method, double amount)
bool process() // succeeds if amount > 0
std::string summary() const
---

Restaurant
uint64_t restId_
std::string name_
ResvManager, WalkInManager, TableManager, OrderManager, Cashier
KticketManager, Expeditor

Restaurant(uint64_t id, std::string name)
reservations(), walkins(), tables(), orders(), cashier(), tickets(), expeditor()
std::string summary() const // state snapshot
---

Result
Purpose: Standardized outcome type for operations across the POS system.

Fields:
bool success — indicates whether the operation succeeded.
std::string message — always contains a message: "Success" or "Failure" by default, but can be overridden with a custom explanation.

Constructors/Factories:
Result(bool s, std::string msg) — stores outcome and message, using move semantics for efficiency.
static Result Ok(const std::string& msg = "Success") — convenience factory for successful results.
static Result Fail(const std::string& msg = "Failure") — convenience factory for failed results.

Design notes:
Constructors throw exceptions only for invariants; mutators and business logic return Result.
Lightweight (just a bool and a string), efficient for older systems.
Provides consistent error handling and clear feedback messages across modules.
Encourages defensive programming and professional, industry‑grade practices.
---

