struct HashPairAStar
{
    template <class T1, class T2>
    std::size_t operator()(const std::pair<T1, T2> &p) const
    {
        auto hash1 = std::hash<T1>{}(p.first);
        auto hash2 = std::hash<T2>{}(p.second);

        // Combining the hashes to produce a unique hash for the pair
        return hash1 ^ hash2;
    }
};
