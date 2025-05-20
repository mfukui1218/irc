#ifndef HOST_HPP
# define HOST_HPP

/**
 * 何が必要かわかってないけどたぶん必要なため作った。
 */
class Host
{
	public:
		Host();
		Host(const Host &other);
		virtual ~Host();
		Host operator=(const Host &rhs);
};

#endif // HOST_HPP
