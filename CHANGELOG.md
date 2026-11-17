# Changelog

## 1.5.1 (2026-11-18)

### New

- Contract responses expose supported combinations of order types, time in force, and trading sessions through order capabilities.

## 1.5.0 (2026-11-11)

### New

- US option snapshots and option chains provide GTH/CURB extended-session quotes and independent cumulative volume and turnover.
- Protobuf option callbacks provide current-session prices, volume, and turnover with `PreMarket` / `AfterHours` tags and independent cumulative totals.
- Option minute-bar requests support `with_hour_trading` to include or exclude extended-session data while preserving the server default when omitted.
- `NewsClient.get_stock_news` queries stock news with page-number pagination.
- `NewsClient.get_market_news` queries market highlights with cursor pagination.

### Changed

- Protobuf option callbacks use flat quote fields; HTTP snapshots and option chains retain their extended-session objects.

## 1.4.0 (2026-09-22)

### New

- Asset and position queries support `AssetQuoteType` values `ETH`, `RTH`, and `OVERNIGHT` for session-specific valuations. Overnight valuations apply to US stocks.
- `get_prime_asset`, `get_prime_portfolio`, `get_asset`, `get_positions`, `get_position_list`, and `get_aggregate_assets` provide quote-basis overloads.
- `IPushClient.subscribe_asset` and `subscribe_position` support quote-basis selection.

### Changed

- Existing method signatures and request behavior remain unchanged when no quote basis is specified; the server uses `ETH`.

## 1.3.2 (2026-08-27)

### New

- Real-time stock and cryptocurrency quotes provide turnover.
- `get_quote_real_time` provides a `SecType` overload for cryptocurrency quotes.

## 1.3.1 (2026-08-19)

### New

- K-line and current-timeline queries support cryptocurrency symbols through `SecType::CC`, including K-line pagination.
- `KlineItem.volume_decimal` provides fractional volume.
- Option chains provide current and previous mark prices, midpoint prices, and their timestamps.
- `TradingSession::OverNight` enables overnight queries for K-lines, depth, and trade ticks.

### Fixed

- `get_quote_overnight` uses the required API version 1.0.

### Breaking

- Adding `KlineItem::volume_decimal` changes the exported structure layout. Rebuild applications and libraries against the updated SDK.

## 1.3.0 (2026-07-24)

### New

- `OrderUtil::amount_order` creates market orders by monetary amount.
- `OrderUtil::multi_leg_order` creates multi-leg orders, including option spreads.
- `OrderUtil::twap_order` and `vwap_order` create time-sliced algorithmic orders.
- `OrderUtil::oca_brackets_order` creates paired profit-taking and stop-loss orders with automatic cancellation after either fills.
- Attached-order helpers add profit-taking, stop-loss, stop-limit, trailing-stop, and bracket orders to an existing order.
- Order placement, modification, and preview support the new multi-leg, algorithmic, and attached-order fields. OCA brackets and attached-order helpers require `place_order` and do not support `preview_order`.

### Changed

- HTTP requests reuse connections more efficiently.
- All platforms require C++14 or later and support C++17 applications. Supported dependency versions are Boost 1.86.0, Protobuf 5.28.3, and Abseil 20240722.0.

### Fixed

- OCA bracket child orders now include the account required for submission.

## 1.2.6 (2026-07-24)

### New

- `OrderUtil::iceberg_order` creates iceberg orders with basic and extended options.
- Order placement and modification support iceberg-order fields.

### Fixed

- Order responses correctly parse quantity scales for fractional-share and other scaled-quantity orders.

## 1.2.5 (2026-07-23)

### New

- `ClientConfig::start_token_refresh` and `stop_token_refresh` manage automatic token refresh with configurable intervals and file persistence.
- Properties configuration supports custom service URLs and response-signature verification keys.

### Fixed

- Concurrent logging no longer causes crashes.
- Request failures retain their original exception details.

## 1.2.4 (2026-07-20)

### New

- `get_corporate_symbol_change` queries stock symbol changes.
- `get_corporate_delisting` queries delisting events.
- `get_corporate_ipo` queries IPO events.

### Breaking

- `get_corporate_action` now requires symbols, market, action type, and date-range arguments. Replace calls such as `client.get_corporate_action()` with:

```cpp
value symbols = value::array();
symbols[0] = value::string(U("AAPL"));
client.get_corporate_action(symbols, U("US"), U("dividend"),
                           U("2024-01-01"), U("2024-12-31"));
```

## 1.2.3 (2026-06-15)

### New

- Option-exercise APIs support exercise and lapse requests, cancellation, position-change checks, paginated records, and eligible-position queries.

### Fixed

- `json_format` safely handles empty strings.
- `identifiers_to_options` handles sparse arrays correctly.
- `send_request` handles previously failing response shapes.
- Signature generation correctly handles non-string fields.

## 1.2.2 (2026-05-07)

### New

- Order-status push messages provide time in force, order-information update time, and order-status update time.

## 1.2.1 (2026-04-13)

### New

- Configuration supports PKCS#8 and PKCS#1 private keys, preferring PKCS#8.
- Configuration paths accept either a file or a directory.
- Examples read configuration from `~/.tigeropen/` by default.

## 1.2.0

### New

- Option and futures quote APIs support symbol lists, depth, timelines, option analysis, and historical main-contract mappings.
- Fund quote APIs support symbols, contracts, current quotes, and history.
- Financial-data APIs support currencies and exchange rates.
- Stock quote APIs add fundamentals, trading rankings, overnight quotes, broker holdings, scanner tags, and additional filtering options.
- Trading APIs add order previews, aggregate assets, funding-transfer history and details, and position transfers with transfer records.
- Push subscriptions support cryptocurrency quotes, heartbeat callbacks, and metrics for top-ranked instruments.
- Cryptocurrency data types and service constants support the new APIs.

## 1.1.0 (2025-08-08)

### New

- Added TBUS configuration support.

## 1.0.4 (2025-05-12)

### Breaking

- Push subscription methods now return an `unsigned int` request ID instead of a boolean.

## 1.0.3 (2025-02-20)

### New

- Added properties-file configuration and kickout callbacks.

### Changed

- Improved logging configuration.

## 1.0.2 (2025-01-15)

### Fixed

- Fixed DNS resolution for persistent push connections.

## 1.0.1 (2025-01-02)

### Changed

- Maintenance release.

## 1.0.0 (2024-11-28)

### New

- Added persistent push connections.

## 0.1.6 (2024-03-08)

### New

- `PriceUtil` adjusts order prices to the applicable tick size.

## 0.1.5 (2024-02-04)

### New

- Trading APIs support `secret_key`.

## 0.1.0 (2023-02-15)

### New

- Initial beta release with trading and quote APIs.
