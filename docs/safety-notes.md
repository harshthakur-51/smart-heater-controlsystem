# Safety Notes

This repository documents a smart-heater controller at a portfolio and architecture level. It is not a consumer build guide and must not be used to assemble or repair mains-powered equipment without qualified engineering review.

## Public Documentation Boundary

Safe to publish:

- high-level architecture;
- sanitized firmware state machines;
- non-confidential component-level discussion;
- validation checklist categories;
- lessons learned from redesign.

Review before publishing:

- production schematics;
- PCB source files and Gerbers;
- BOM pricing and supplier data;
- client names or manufacturing details;
- firmware copied from commercial products;
- safety test records or certification documents.

Do not publish:

- customer-owned files without permission;
- exact manufacturing package if it is proprietary;
- any credentials, serial numbers, or internal part numbers that identify a client product.

## Engineering Safety Checklist

A mains-powered heater controller needs validation for:

- creepage and clearance;
- reinforced/basic insulation boundaries;
- fuse coordination;
- MOV and surge protection sizing;
- X2 capacitor safety rating;
- transformer insulation and hipot testing;
- optocoupler isolation rating;
- triac thermal margin;
- thermal fuse placement;
- NTC open/short detection;
- firmware watchdog and brownout behavior;
- fail-safe heater-off default state.
