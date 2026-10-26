CREATE TABLE Artists (
    artist_id INTEGER PRIMARY KEY,
    name TEXT NOT NULL UNIQUE
);

CREATE TABLE Concerts (
    concert_id INTEGER PRIMARY KEY,
    venue TEXT NOT NULL,
    city TEXT NOT NULL,
    event_date DATE NOT NULL,
    cost INTEGER NOT NULL
);

CREATE TABLE ConcertArtists (
    concert_id INTEGER NOT NULL REFERENCES Concerts(concert_id),
    artist_id INTEGER NOT NULL REFERENCES Artists(artist_id),
    role TEXT NOT NULL CHECK (role IN ('Headline', 'Support', 'Guest')),
    PRIMARY KEY (concert_id, artist_id)
);
