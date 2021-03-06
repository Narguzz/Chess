#ifndef GAME_H
#define GAME_H

#include "Move.h"
#include "MoveGenerator.h"
#include "Hashing.h"

struct Undo
{
	Move move;

	u8* targetSquare;
	PieceType targetType;

	u8 halfmoveClock;
	u8 enPassantSquare;
	u8 lastMovedPieceSquare;
	u8 castlingRights;
};

class Game
{
public:
	Game();
	Game(const Game&);

	~Game();

	u64 occupancy() const;

	u64 player(Player) const;
	u64 pieces(PieceType) const;
	u64 piecesOf(Player, PieceType) const;
	u8 pieceType(u8) const;

	Player activePlayer() const;
	u8 castlingRights() const;
	u8 enPassantSquare() const;
	u8 lastMovedSquare() const;

	u64 hash() const;

	const std::list<Move>& possibleMoves() const;

	Status status() const;

	bool isOver() const;
	bool isKingInCheck(Player) const;

	void makeMove(const Move&);
	void unmakeMove();

private:
	static std::array<i8, 2> sPawnShift;
	static std::array<u64, 2> sDoublePushMask;
	static std::array<u64, 2> sPromotionMask;

	static std::array<u8, 2> sCastleDelta;
	static std::array<u8, 2> sCastleShift;
	static std::array<u8, 2> sCastleRookFrom;
	static std::array<u8, 2> sCastleRookTo;


	u64 _makeMove(const Move&);
	void _unmakeMove();

	u64 _movePiece(u8, u8, Player);
	u64 _addPiece(u8, PieceType, Player);
	u64 _removePiece(u8, PieceType, Player);

	void _refreshKingSquare(Player);

	void _generateMoves();


	void _addPawnMoves(Player);
	void _addKnightMoves(Player);
	void _addBishopMoves(Player);
	void _addRookMoves(Player);
	void _addQueenMoves(Player);
	void _addKingMoves(Player);

	void _addMovesFrom(u8, u64, MoveType);
	void _addMovesShift(i8, u64, MoveType);
	void _addPromoShift(i8, u64);
	void _addPromoCaptureShift(i8, u64);

	bool _isAttacked(u8, Player) const;

	bool _canCastleKingSide(Player) const;
	bool _canCastleQueenSide(Player) const;


	u64 mOccupancy;
	std::array<u64, 2> mPlayers;
	std::array<u64, 6> mPieces;

	std::array<u8, 2> mKings;

	std::array<u8, 64> mPieceTypes;

	Player mActivePlayer;
	u8 mHalfmoveClock;
	u8 mEnPassantSquare;
	u8 mLastMovedPieceSquare;
	u8 mCastlingRights;

	Status mStatus;

	std::stack<std::list<Move>> mMoves;
	std::vector<Undo> mHistory;
	std::stack<u64> mHashs;

	std::map<u64, u8> mHashsVisits;
};


void perft(Game*, u64&, int);

#endif // GAME_H
